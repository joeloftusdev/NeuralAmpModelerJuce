# NeuralAmpModelerJuce
![Screenshot 2024-12-11 200402](https://github.com/user-attachments/assets/760fffe1-f8a7-4a45-bbe7-f29da121d497)

```
git clone --recurse-submodules <repository_url>
cmake -B build -DCMAKE_BUILD_TYPE=Release
cmake --build build --config Release
```

I recommend building with Clang, I have added specific clang optimisations to cmake which targets the `-march=x86-64-v3` architecture used on modern cpus.

# Acknowledgments

- Steven Atkinson & Neural Amp Modeler: This project would not exist without the original NAM.

