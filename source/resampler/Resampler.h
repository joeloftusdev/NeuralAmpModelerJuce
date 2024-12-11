#pragma once

#define DEFAULT_BLOCK_SIZE 256

namespace iplug
{
static constexpr double PI = 3.1415926535897931;
};

#include <algorithm> // std::clamp, std::min
#include <cmath> // pow
#include <filesystem>
#include <utility>
#include "nam-core/NAM/dsp.h"
#include "nam-resampler/ResamplingContainer.h"


static double GetNAMSampleRate(const std::unique_ptr<nam::DSP>& model)
{
    constexpr double assumedSampleRate = 48000.0;
    const double reportedEncapsulatedSampleRate = model->GetExpectedSampleRate();
    const double encapsulatedSampleRate = reportedEncapsulatedSampleRate <= 0.0 ? assumedSampleRate : reportedEncapsulatedSampleRate;
    return encapsulatedSampleRate;
};

class ResamplingNAM final : public nam::DSP
{
public:
    ResamplingNAM(std::unique_ptr<DSP> encapsulated, const double expected_sample_rate)
        : DSP(expected_sample_rate), mEncapsulated(std::move(encapsulated)), mResampler(GetNAMSampleRate(mEncapsulated))
    {
        auto ProcessBlockFunc = [&](NAM_SAMPLE** input, NAM_SAMPLE** output,const int numFrames)
        {
            mEncapsulated->process(input[0], output[0], numFrames);
            mEncapsulated->finalize_(numFrames);
        };
        mBlockProcessFunc = ProcessBlockFunc;

        if (mEncapsulated->HasLoudness())
            SetLoudness(mEncapsulated->GetLoudness());
        int maxBlockSize = 2048; // Conservative
        Reset(expected_sample_rate, maxBlockSize);
    }

    ~ResamplingNAM() override = default;

    void prewarm() override { mEncapsulated->prewarm(); }

    void process(NAM_SAMPLE* input, NAM_SAMPLE* output, const int num_frames) override
    {
        if (!mFinalized)
            throw std::runtime_error("Processing was called before the last block was finalized!");
        if (num_frames > mMaxExternalBlockSize)
            throw std::runtime_error("More frames were provided than the max expected!");

        if (!NeedToResample())
        {
            mEncapsulated->process(input, output, num_frames);
            mEncapsulated->finalize_(num_frames);
        }
        else
        {
            mResampler.ProcessBlock(&input, &output, num_frames, mBlockProcessFunc);
        }
        lastNumExternalFramesProcessed = num_frames;
        mFinalized = false;
    };

    void finalize_(const int num_frames) override
    {
        if (mFinalized)
            throw std::runtime_error("Call to ResamplingNAM.finalize_() when the object is already in a finalized state!");
        if (num_frames != lastNumExternalFramesProcessed)
            throw std::runtime_error(
                "finalize_() called on ResamplingNAM with a different number of frames from what was just processed. Something "
                "is probably going wrong.");
        mFinalized = true;
    };

    [[nodiscard]] int GetLatency() const { return NeedToResample() ? mResampler.GetLatency() : 0; };

    void Reset(const double sampleRate, const int maxBlockSize)
    {
        mExpectedSampleRate = sampleRate;
        mMaxExternalBlockSize = maxBlockSize;
        mResampler.Reset(sampleRate, maxBlockSize);

        const double mUpRatio = sampleRate / GetEncapsulatedSampleRate();
        const auto maxEncapsulatedBlockSize = static_cast<int>(std::ceil(static_cast<double>(maxBlockSize) / mUpRatio));
        std::vector<NAM_SAMPLE> input, output;
        for (int i = 0; i < maxEncapsulatedBlockSize; i++)
            input.push_back(0.0);
        output.resize(maxEncapsulatedBlockSize);
        mEncapsulated->process(input.data(), output.data(), maxEncapsulatedBlockSize);
        mEncapsulated->finalize_(maxEncapsulatedBlockSize);

        mFinalized = true;
    };

    [[nodiscard]] double GetEncapsulatedSampleRate() const { return GetNAMSampleRate(mEncapsulated); };

private:
    [[nodiscard]] bool NeedToResample() const { return GetExpectedSampleRate() != GetEncapsulatedSampleRate(); };
    std::unique_ptr<DSP> mEncapsulated;
    bool mFinalized = true;
    dsp::ResamplingContainer<NAM_SAMPLE, 1> mResampler;
    int mMaxExternalBlockSize = 0;
    int lastNumExternalFramesProcessed = -1;
    std::function<void(NAM_SAMPLE**, NAM_SAMPLE**, int)> mBlockProcessFunc;
};

