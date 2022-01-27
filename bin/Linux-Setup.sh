
for config in Debug Release Dist
do
    mkdir -p build/$config-linux-x86_64/Article
    cp Lithium/vendor/SDL2/lib/linux-x86_64/libSDL2.so build/$config-linux-x86_64/Article
    cp Lithium/vendor/SDL2/lib/linux-x86_64/libSDL2-2.0.so.0 build/$config-linux-x86_64/Article
    cp Lithium/vendor/SDL2/lib/linux-x86_64/libSDL2-2.0.so.0.16.0 build/$config-linux-x86_64/Article

    mkdir -p build/$config-linux-x86_64/AssetBase
    cp AssetBase/vendor/ShaderConductor/lib/Debug-linux-x86_64/lib*.so build/$config-linux-x86_64/AssetBase
    cp AssetBase/vendor/ShaderConductor/lib/Debug-linux-x86_64/lib*.so.3.7 build/$config-linux-x86_64/AssetBase
done
