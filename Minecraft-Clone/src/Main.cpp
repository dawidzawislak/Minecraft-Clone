#include "Game.h"
#include "VertexArray.h"
#include "IndexBuffer.h"
#include "Shader.h"
#include "Texture.h"

#include "BlockTextureManager.h"
#include <iostream>

int main()
{
    Game game("test", 1280, 720);
    game.Run();
    
    return 0;
}