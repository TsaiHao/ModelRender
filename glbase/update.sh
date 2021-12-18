git clone --single-branch -b 3.3-stable git@github.com:glfw/glfw.git thirdparty/glfw

git clone --single-branch -b master --no-tags git@github.com:g-truc/glm.git thirdparty/glm
#wget https://github.com/g-truc/glm/releases/download/0.9.9.8/glm-0.9.9.8.7z
#unzip glm-0.9.9.8.zip thirdparty/glm

wget https://glad.dav1d.de/generated/tmpii649be6glad/glad.zip glad.zip
unzip glad.zip -d thirdparty/glad
rm -f glad.zip