#include "object_manager.hpp"

#include "game.hpp"
#include <queue>
#include "collision_calculator.hpp"
#include "curved_shape.hpp"
#include "earcut.hpp"
#include "jump_pad.hpp"
#include <iostream>

#define norm(vec) (std::sqrt(vec.x * vec.x + vec.y * vec.y))
#define dot(v1, v2) (v1.x * v2.x + v1.y * v2.y)

const sf::View ObjectManager::defaultView = sf::View(
    {0, 0},
    {(float)Game::getSettings().getInt("Screen", "cameraWidth", 640), (float)Game::getSettings().getInt("Screen", "cameraWidth", 360)});

void ObjectManager::load(const std::string &path)
{
    auto &settings = Game::getSettings();
    sf::Vector2f cameraSize = {(float)settings.getInt("Screen", "cameraWidth", 640), (float)settings.getInt("Screen", "cameraWidth", 360)};
    camera = std::make_shared<Camera>(cameraSize);
    background = std::make_shared<Background>();
    player = std::make_shared<Player>();
    player->setPosition(500, -500);
    camera->setFollowObject(player);
    std::vector<std::array<double, 2>> verttts = {{69, 143}, {69, 157}, {68, 169}, {69, 181}, {69, 191}, {69, 201}, {69, 209}, {69, 217}, {69, 223}, {69, 229}, {69, 235}, {69, 240}, {69, 244}, {69, 248}, {69, 252}, {69, 256}, {69, 259}, {69, 263}, {69, 266}, {69, 270}, {69, 274}, {69, 278}, {69, 283}, {69, 289}, {69, 295}, {69, 295}, {69, 301}, {69, 309}, {69, 317}, {69, 327}, {69, 336}, {69, 347}, {69, 358}, {69, 370}, {69, 382}, {69, 395}, {69, 408}, {69, 421}, {69, 435}, {69, 449}, {69, 462}, {69, 476}, {69, 490}, {69, 504}, {69, 518}, {69, 531}, {69, 545}, {69, 558}, {69, 570}, {70, 583}, {70, 583}, {69, 595}, {69, 610}, {69, 627}, {69, 645}, {68, 664}, {68, 685}, {67, 707}, {67, 730}, {66, 754}, {65, 778}, {65, 804}, {64, 830}, {63, 856}, {62, 883}, {61, 910}, {61, 938}, {60, 965}, {59, 992}, {58, 1019}, {57, 1046}, {57, 1072}, {56, 1097}, {55, 1122}, {55, 1146}, {55, 1146}, {54, 1168}, {55, 1188}, {58, 1206}, {61, 1223}, {65, 1238}, {69, 1251}, {75, 1263}, {82, 1273}, {89, 1282}, {97, 1289}, {106, 1295}, {115, 1301}, {125, 1305}, {135, 1309}, {146, 1311}, {158, 1313}, {169, 1315}, {181, 1316}, {193, 1316}, {206, 1316}, {219, 1317}, {232, 1316}, {245, 1316}, {258, 1317}, {258, 1317}, {270, 1317}, {283, 1317}, {295, 1317}, {308, 1317}, {320, 1318}, {332, 1318}, {343, 1318}, {355, 1319}, {366, 1319}, {377, 1319}, {389, 1319}, {400, 1319}, {411, 1319}, {422, 1319}, {432, 1318}, {443, 1318}, {454, 1317}, {465, 1316}, {476, 1315}, {487, 1314}, {497, 1312}, {508, 1310}, {519, 1308}, {531, 1306}, {531, 1306}, {542, 1303}, {554, 1299}, {566, 1296}, {579, 1291}, {593, 1287}, {607, 1282}, {621, 1277}, {636, 1271}, {650, 1265}, {666, 1259}, {681, 1253}, {697, 1247}, {712, 1240}, {728, 1233}, {744, 1226}, {760, 1220}, {776, 1213}, {792, 1206}, {807, 1199}, {823, 1192}, {838, 1185}, {854, 1178}, {869, 1171}, {884, 1165}, {884, 1165}, {898, 1158}, {912, 1151}, {926, 1145}, {940, 1138}, {954, 1131}, {967, 1124}, {980, 1117}, {994, 1110}, {1007, 1103}, {1020, 1096}, {1033, 1089}, {1046, 1082}, {1059, 1075}, {1073, 1068}, {1086, 1062}, {1099, 1055}, {1113, 1049}, {1127, 1042}, {1140, 1036}, {1154, 1030}, {1169, 1025}, {1183, 1019}, {1198, 1014}, {1214, 1009}, {1214, 1009}, {1228, 1004}, {1240, 999}, {1250, 995}, {1258, 991}, {1265, 987}, {1270, 984}, {1274, 981}, {1276, 978}, {1277, 976}, {1276, 974}, {1275, 971}, {1273, 970}, {1270, 968}, {1267, 967}, {1263, 965}, {1259, 964}, {1254, 963}, {1250, 963}, {1245, 962}, {1241, 962}, {1236, 962}, {1233, 961}, {1229, 961}, {1227, 962}, {1227, 962}, {1222, 962}, {1215, 964}, {1204, 966}, {1191, 969}, {1175, 973}, {1157, 978}, {1137, 983}, {1115, 988}, {1093, 994}, {1069, 1000}, {1045, 1006}, {1020, 1012}, {996, 1018}, {971, 1024}, {947, 1030}, {925, 1035}, {903, 1040}, {883, 1045}, {865, 1049}, {849, 1052}, {835, 1054}, {824, 1056}, {816, 1056}, {812, 1056}, {812, 1056}, {808, 1054}, {805, 1053}, {801, 1052}, {796, 1050}, {792, 1049}, {787, 1047}, {782, 1045}, {777, 1043}, {773, 1041}, {768, 1039}, {764, 1037}, {760, 1035}, {757, 1032}, {754, 1029}, {752, 1027}, {751, 1024}, {751, 1020}, {751, 1017}, {753, 1014}, {756, 1010}, {760, 1006}, {766, 1002}, {773, 998}, {782, 994}, {782, 994}, {791, 989}, {802, 983}, {812, 977}, {823, 971}, {834, 964}, {845, 958}, {856, 951}, {866, 944}, {876, 937}, {885, 930}, {893, 924}, {900, 918}, {905, 912}, {910, 907}, {913, 902}, {914, 899}, {913, 896}, {910, 894}, {905, 892}, {897, 893}, {887, 894}, {874, 896}, {858, 900}, {840, 906}, {840, 906}, {820, 911}, {801, 915}, {784, 918}, {768, 920}, {753, 920}, {738, 920}, {725, 919}, {712, 917}, {701, 915}, {689, 912}, {679, 909}, {669, 905}, {659, 902}, {650, 898}, {641, 894}, {633, 890}, {624, 887}, {616, 884}, {607, 881}, {599, 879}, {590, 877}, {582, 876}, {573, 877}, {564, 878}, {564, 878}, {554, 878}, {544, 879}, {534, 878}, {523, 877}, {513, 875}, {502, 873}, {491, 871}, {481, 869}, {471, 868}, {461, 867}, {451, 866}, {441, 867}, {432, 869}, {424, 871}, {416, 876}, {409, 882}, {402, 889}, {396, 899}, {391, 910}, {387, 924}, {384, 941}, {382, 960}, {381, 982}, {382, 1007}, {382, 1007}, {381, 1031}, {378, 1052}, {372, 1070}, {365, 1085}, {355, 1097}, {344, 1106}, {331, 1112}, {317, 1116}, {302, 1118}, {287, 1118}, {271, 1115}, {254, 1111}, {238, 1106}, {222, 1099}, {207, 1090}, {192, 1081}, {178, 1071}, {166, 1060}, {155, 1048}, {145, 1036}, {138, 1024}, {132, 1011}, {130, 999}, {130, 988}, {130, 988}, {130, 976}, {131, 964}, {131, 951}, {130, 939}, {129, 926}, {128, 914}, {127, 901}, {126, 888}, {124, 876}, {123, 863}, {122, 851}, {121, 840}, {120, 828}, {119, 817}, {119, 806}, {119, 796}, {120, 786}, {122, 777}, {124, 769}, {127, 761}, {130, 755}, {135, 749}, {141, 744}, {148, 740}, {148, 740}, {155, 736}, {162, 733}, {169, 730}, {176, 728}, {183, 726}, {189, 724}, {195, 723}, {201, 722}, {206, 720}, {211, 720}, {215, 719}, {218, 718}, {220, 718}, {222, 717}, {222, 717}, {222, 716}, {221, 716}, {218, 715}, {214, 715}, {209, 714}, {203, 713}, {195, 712}, {186, 711}, {176, 710}, {176, 710}, {165, 708}, {155, 705}, {147, 702}, {140, 699}, {134, 695}, {129, 690}, {125, 685}, {122, 680}, {119, 674}, {118, 668}, {117, 662}, {116, 655}, {116, 649}, {117, 642}, {117, 634}, {118, 627}, {119, 620}, {120, 612}, {122, 605}, {123, 597}, {123, 589}, {124, 582}, {125, 575}, {125, 568}, {125, 568}, {124, 559}, {124, 550}, {124, 539}, {124, 526}, {125, 512}, {125, 496}, {125, 480}, {126, 462}, {126, 444}, {127, 425}, {127, 405}, {128, 385}, {128, 364}, {129, 343}, {130, 321}, {130, 300}, {131, 278}, {132, 257}, {132, 235}, {133, 214}, {134, 194}, {134, 174}, {135, 155}, {136, 137}, {136, 137}, {136, 119}, {135, 104}, {134, 91}, {132, 79}, {130, 69}, {127, 60}, {123, 53}, {120, 48}, {116, 44}, {112, 42}, {108, 41}, {104, 41}, {99, 43}, {95, 46}, {91, 51}, {87, 56}, {83, 63}, {80, 72}, {76, 81}, {74, 91}, {72, 102}, {70, 115}, {69, 128}, {69, 143}};

    ground.push_back(std::make_shared<Ground>(verttts));
    auto ptr = std::make_shared<JumpPad>();
    ptr->setPosition(750, 320);
    objects.push_back(ptr);
    collidables.push_back(ptr);
}

void ObjectManager::save(const std::string &path)
{
}

void ObjectManager::updateAll()
{
    if (sf::Mouse::isButtonPressed(sf::Mouse::Button::Left))
    {
        player->setPosition(Game::getMousePos());
        player->getPhysical()->speed = {0, 0};
    }
    recursiveUpdate(camera);
    recursiveUpdate(background);
    for (auto obj : objects)
        recursiveUpdate(obj);
    recursiveUpdate(player);
}

void ObjectManager::collideAll()
{
    for (auto grd : ground)
        calculateCollision(player, grd);
    for (auto cld : collidables)
        calculateCollision(player, cld);
}

void ObjectManager::moveAll()
{
    for (auto obj : objects)
        recursiveMove(obj);
    recursiveMove(player);
}

void ObjectManager::drawAll(sf::RenderTarget &target)
{
    target.setView(defaultView);
    recursiveDraw(background, target);
    target.setView(this->getCamera());
    for (auto grd : ground)
        recursiveDraw(grd, target);
    for (auto obj : objects)
        recursiveDraw(obj, target);
    recursiveDraw(player, target);
}

sf::View ObjectManager::getCamera() const
{
    if (camera)
        return camera->getView();
    return sf::View();
}

void ObjectManager::recursiveDraw(std::shared_ptr<GameObject> object, sf::RenderTarget &target)
{
    sf::Transform identity = sf::Transform::Identity;
    std::queue<std::shared_ptr<GameObject>> objects;
    std::queue<sf::Vector2f> positions;
    std::queue<float> rotations;
    objects.push(object);
    positions.push(sf::Vector2f(0, 0));
    rotations.push(0.0f);
    while (objects.size() != 0)
    {
        std::shared_ptr<GameObject> cur = objects.front();
        if (cur->getDrawable())
            target.draw(*cur->getDrawable(), identity.translate(positions.front()).rotate(rotations.front()) * cur->getTransform());
        for (auto child : cur->getChildren())
        {
            objects.push(child);
            positions.push(positions.front() + cur->getPosition());
            rotations.push(rotations.front() + cur->getRotation());
        }
        objects.pop();
        positions.pop();
        rotations.pop();
    }
}

void ObjectManager::recursiveUpdate(std::shared_ptr<GameObject> object)
{
    std::queue<std::shared_ptr<GameObject>> objects;
    objects.push(object);
    while (objects.size() != 0)
    {
        std::shared_ptr<GameObject> cur = objects.front();
        cur->update();
        for (auto child : cur->getChildren())
            objects.push(child);
        objects.pop();
    }
}

void ObjectManager::recursiveMove(std::shared_ptr<GameObject> object)
{
    std::queue<std::shared_ptr<GameObject>> objects;
    objects.push(object);
    while (objects.size() != 0)
    {
        std::shared_ptr<GameObject> cur = objects.front();
        if (cur->getPhysical())
            cur->move(cur->getPhysical()->speedUpdate());
        for (auto child : cur->getChildren())
            objects.push(child);
        objects.pop();
    }
}

void ObjectManager::recursiveCollision(std::shared_ptr<GameObject> first, std::shared_ptr<GameObject> second)
{
    // std::queue<std::shared_ptr<GameObject>> firstObjects;
    // firstObjects.push(first);
    // while (firstObjects.size() != 0)
    // {
    //     std::shared_ptr<GameObject> firstCur = firstObjects.front();

    //     // Another BFS
    //     std::queue<std::shared_ptr<GameObject>> secondObjects;
    //     secondObjects.push(second);
    //     while (secondObjects.size() != 0)
    //     {
    //         std::shared_ptr<GameObject> secondCur = secondObjects.front();
    //         if (firstCur->getCollidable() && secondCur->getCollidable() && !(firstCur->getCollidable()->isTrigger() && secondCur->getCollidable()->isTrigger()))
    //             calculateCollision(firstCur, secondCur);
    //         for (auto secondChild : secondCur->getChildren())
    //             secondObjects.push(secondChild);
    //         secondObjects.pop();
    //     }
    //     // ==========================

    //     for (auto firstChild : firstCur->getChildren())
    //         firstObjects.push(firstChild);
    //     firstObjects.pop();
    // }
}

void ObjectManager::calculateCollision(std::shared_ptr<GameObject> first, std::shared_ptr<GameObject> second, bool notify)
{
    const Hitbox &firstHitbox = first->getCollidable()->getHitbox();
    const Hitbox &secondHitbox = second->getCollidable()->getHitbox();

    switch (secondHitbox.index())
    {
    case 0: // circle

    case 1: // convex
    {
        auto fCollidable = first->getCollidable();
        auto fPhysical = first->getPhysical();
        auto sCollidable = second->getCollidable();
        auto sPhysical = second->getPhysical();
        auto simplex = collision_calculator::getGJKCollisionSimplex(firstHitbox, secondHitbox);
        if (simplex.size() == 0)
            return;
        if (notify)
        {
            first->onCollision(second);
            second->onCollision(first);
            notify = false;
        }
        if (!fPhysical || sCollidable->isTrigger())
            return;
        sf::Vector2f penetration = -collision_calculator::getPenetrationVector(firstHitbox, secondHitbox, simplex);

        // first->move(penetration);
        // penetration = (1 / norm(penetration)) * penetration;
        // float vn = dot(fPhysical->speed, penetration);
        // if (vn < 0)
        //     fPhysical->speed -= penetration * vn;

        first->move(penetration);
        fPhysical->speed += penetration;

        break;
    }

    case 2: // concave
    {
        auto fCollidable = first->getCollidable();
        auto fPhysical = first->getPhysical();
        auto sCollidable = second->getCollidable();
        auto sPhysical = second->getPhysical();
        auto &triangles = std::get<ConcaveHitbox>(secondHitbox).triangles;

        // for (size_t i = 0; i < 10; ++i)
        // {
        //     sf::Vector2f minPenetration = {99999.0f, 99999.0f};
        //     for (auto &triangle : triangles)
        //     {
        //         auto simplex = collision_calculator::getGJKCollisionSimplex(firstHitbox, triangle);
        //         if (simplex.size() == 0)
        //             continue;
        //         if (notify)
        //         {
        //             first->onCollision(second);
        //             second->onCollision(first);
        //             notify = false;
        //             if (!fPhysical || sCollidable->isTrigger())
        //                 return;
        //         }
        //         sf::Vector2f penetration = -collision_calculator::getPenetrationVector(firstHitbox, triangle, simplex);
        //         if (norm(penetration) > 0.5 && norm(penetration) < norm(minPenetration))
        //             minPenetration = penetration;
        //     }
        //     if (minPenetration.x == 99999.0f)
        //         return;
        //     first->move(minPenetration);
        //     minPenetration = (1 / norm(minPenetration)) * minPenetration;
        //     float vn = dot(fPhysical->speed, minPenetration);
        //     if (vn < 0)
        //         fPhysical->speed -= minPenetration * vn;
        // }

        sf::Vector2f penetration = {0, 0};
        size_t count = 0;
        for (auto triangle : triangles)
        {
            auto simplex = collision_calculator::getGJKCollisionSimplex(firstHitbox, triangle);
            if (simplex.size() == 0)
                continue;
            if (notify)
            {
                first->onCollision(second);
                second->onCollision(first);
                notify = false;
                if (!fPhysical || sCollidable->isTrigger())
                    return;
            }
            penetration -= collision_calculator::getPenetrationVector(firstHitbox, triangle, simplex);
            count++;
        }
        if (count == 0)
            return;
        penetration = {penetration.x / count, penetration.y / count};
        first->move(penetration);
        fPhysical->speed += penetration;
        
        break;
    }

    default:
        break;
    }
}