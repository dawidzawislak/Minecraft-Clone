#include "Game.h"
#include "VertexArray.h"
#include "IndexBuffer.h"
#include "Shader.h"
#include "Texture.h"

#include "BlockTextureManager.h"
#include <iostream>

#include "ECS.h"

int main()
{
    Game game("test", 1280, 720);
    game.Run();
    /*
    EntityID e1 = EntityRegisty::NewEntity();
    EntityID e2 = EntityRegisty::NewEntity();
    EntityID e3 = EntityRegisty::NewEntity();
    
    printf_s("e1: %llu\n", e1);
    printf_s("e2: %llu\n", e2);
    printf_s("e3: %llu\n", e3);

    Transform* t1 = EntityRegisty::AssignComponent<Transform>(e1);
    std::cout << "\nt1: " << EntityRegisty::GetComponent<Transform>(e1)->pos;
    t1->pos = 5;

    EntityRegisty::AssignComponent<Transform>(e2);

    EntityRegisty::AssignComponent<Shape>(e2);

    EntityRegisty::AssignComponent<Shape>(e3);
    
    printf_s("\nTransform & Shape\n");
    for (EntityID id : RegistryView<Transform, Shape>())
        printf_s("%llu\n", id);

    printf_s("\nTransform\n");
    for (EntityID id : RegistryView<Transform>())
        printf_s("%llu\n", id);

    printf_s("\nShape\n");
    for (EntityID id : RegistryView<Shape>())
        printf_s("%llu\n", id);

    EntityRegisty::RemoveComponent<Shape>(e2);

    EntityRegisty::DestroyEntity(e3);

    printf_s("\nShape\n");
    for (EntityID id : RegistryView<Shape>())
        printf_s("%llu\n", id);

    std::cout << "\nshape ID: " << GetComponentID<Shape>();
    std::cout << "\nTransform ID: " << GetComponentID<Transform>();

    EntityRegisty::Release();
    */
    return 0;
}