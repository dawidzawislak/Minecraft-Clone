#include "Game.h"
#include "VertexArray.h"
#include "IndexBuffer.h"
#include "Shader.h"
#include "Texture.h"

#include "BlockTextureManager.h"
#include <iostream>

int main()
{
    Game game("Minecraft Clone", 1280, 720);
    game.Run();
    /*
    std::cout << "LU:   " << btm.GetUVLU("grass_block_side").x << "   " << btm.GetUVLU("grass_block_side").y << std::endl;
    std::cout << "LD:   " << btm.GetUVLD("grass_block_side").x << "   " << btm.GetUVLD("grass_block_side").y << std::endl;
    std::cout << "RD:   " << btm.GetUVRD("grass_block_side").x << "   " << btm.GetUVRD("grass_block_side").y << std::endl;
    std::cout << "RU:   " << btm.GetUVRU("grass_block_side").x << "   " << btm.GetUVRU("grass_block_side").y << std::endl;
    */
    /*
    FastNoiseLite gen;

double noise(double nx, double ny) { // if using fastnoiselite
    // Rescale from -1.0:+1.0 to 0.0:1.0
    return gen.GetNoise(nx, ny) / 2.0 + 0.5;}
    glm::mat4 proj = glm::perspective(glm::radians(45.0f), (float)WND_WIDTH / (float)WND_HEIGHT, 0.1f, 100.0f);
    glm::mat4 view = glm::mat4(1.0f);


    float elevation[4][4][16*16];
    for (int y = 0; y < 64; y++) 
        for (int x = 0; x < 64; x++) {
            float e = 1 * noise(1 * x, 1 * y);
            +0.5 * noise(2 * x, 2 * y);
            +0.5 * noise(4 * x, 4 * y);
            elevation[x / 16][y / 16][(x % 16) * 16 + (y % 16)] = e / (2.0);
        }


    Chunk* chunksTab = new Chunk[4*4];

    for (int y = 0; y < 4; y++)
        for (int x = 0; x < 4; x++)
            chunksTab[x*4+y].Initialize(x, y, elevation[x][y]);

    for (int y = 0; y < 4; y++)
        for (int x = 0; x < 4; x++)
            chunksTab[x * 4 + y].Update(x > 0 ? &chunksTab[(x-1) * 4 + y] : nullptr, x < 3 ? &chunksTab[(x + 1) * 4 + y]: nullptr, y > 0 ? &chunksTab[x*4 + y - 1] : nullptr, y < 3 ? &chunksTab[x*4 + y + 1] : nullptr);


    Renderer renderer;

    while (!glfwWindowShouldClose(window)) {
        // --- Update ----------
        float currentFrame = static_cast<float>(glfwGetTime());
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;
        ProcessKeyboardInput(window);

        view = camera.GetViewMatrix();
        glm::mat4 vp = proj * view;

        // --- Draw ------------
        renderer.Clear();

        //renderer.DrawBlocks(blocks, vp);
        renderer.DrawChunks(chunksTab, 4, 4, vp);

        GLCall(glfwSwapBuffers(window));

        GLCall(glfwPollEvents());
    }

    delete[] chunksTab;

    glfwTerminate();

    */
    return 0;
}