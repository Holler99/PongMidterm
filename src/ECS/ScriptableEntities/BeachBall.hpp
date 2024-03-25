#pragma once
#include <string>
#include <vector>
#include <Canis/ScriptableEntity.hpp>
#include <Canis/ECS/Components/RectTransformComponent.hpp>


class BeachBall : public Canis::ScriptableEntity
{
private:
    glm::vec2   m_direction;
    float       m_speed;
    float       m_timeBetweenAnimation = 3.0f;
    float       m_countDown = 0.0f;
    unsigned int m_animIndex = 0;
    std::vector<glm::vec2> m_spawnPoints = {};
    bool m_gameStarted = false;
public:
    void OnCreate()
    {
        m_countDown = m_timeBetweenAnimation;
    }

    void OnReady()
    {
        m_direction = glm::vec2(1.0f, 0.4f);
        m_speed = 150.0f;
    }
    
    void OnDestroy()
    {

    }

    void OnUpdate(float _dt)
    {
        using namespace Canis;
        Entity leftPaddle = entity.GetEntityWithTag("LEFTPADDLE");
        auto& rectLeftPaddle = leftPaddle.GetComponent<RectTransformComponent>(); //could also use auto&
        auto& colorLeftPaddle = leftPaddle.GetComponent<ColorComponent>();
        float leftPaddleSpeed = 200.0f;
        

        Entity rightPaddle = entity.GetEntityWithTag("RIGHTPADDLE");
        auto& rectRightPaddle = rightPaddle.GetComponent<RectTransformComponent>();
        auto& colorRightPaddle = rightPaddle.GetComponent<ColorComponent>();
        float rightPaddleSpeed = 200.0f;

        auto& rect = GetComponent<Canis::RectTransformComponent>(); //the ball
        
        if(!m_gameStarted)
        {
            if (GetInputManager().GetKey(SDL_SCANCODE_RETURN))
            {
                m_gameStarted = true;
                Log("Game Started!");
            } 
            else
            {
                return;
            }
        }

        float halfSizeX = rect.size.x/2.0f;
        float halfSizeY = rect.size.y/2.0f;
        
        // left paddle collision
        if (rect.position.x - halfSizeX <= rectLeftPaddle.position.x + rectLeftPaddle.size.x / 2.0f &&
            rect.position.y >= rectLeftPaddle.position.y - rectLeftPaddle.size.y / 2.0f &&
            rect.position.y <= rectLeftPaddle.position.y + rectLeftPaddle.size.y / 2.0f)
        {
            m_direction.x *= -1.0f; 
        }

        //right paddle collision
         if (rect.position.x + halfSizeX >= rectRightPaddle.position.x - rectRightPaddle.size.x / 2.0f &&
             rect.position.y >= rectRightPaddle.position.y - rectRightPaddle.size.y / 2.0f &&
             rect.position.y <= rectRightPaddle.position.y + rectRightPaddle.size.y / 2.0f)
        {
            m_direction.x *= -1.0f; 
        }

        //top and bottom wall collision
        if (rect.position.x + halfSizeX >= GetWindow().GetScreenWidth()/2.0f ||
                rect.position.x - halfSizeX <= GetWindow().GetScreenWidth()/-2.0f)
            m_direction.x *= -1.0f;

        if (rect.position.y + halfSizeY >= GetWindow().GetScreenHeight()/2.0f ||
                rect.position.y - halfSizeY <= GetWindow().GetScreenHeight()/-2.0f)
            m_direction.y *= -1.0f;
        
        //ball movement
        rect.position += (m_direction * (m_speed * _dt));
        

        if (GetInputManager().GetKey(SDL_SCANCODE_W))
        {
            rectLeftPaddle.position.y += leftPaddleSpeed * _dt;
        }
        
        if (GetInputManager().GetKey(SDL_SCANCODE_S))
        {
            rectLeftPaddle.position.y -= leftPaddleSpeed * _dt;
        }

        if(GetInputManager().GetKey(SDL_SCANCODE_UP))
        {
            rectRightPaddle.position.y += rightPaddleSpeed * _dt;
        }

        if(GetInputManager().GetKey(SDL_SCANCODE_DOWN))
        {
            rectRightPaddle.position.y -= rightPaddleSpeed * _dt;
        }
    }
};

bool DecodeBeachBall(const std::string &_name, Canis::Entity &_entity)
{
    if (_name == "BeachBall")
    {
        Canis::ScriptComponent scriptComponent = {};
        scriptComponent.Bind<BeachBall>();
        _entity.AddComponent<Canis::ScriptComponent>(scriptComponent);
        return true;
    }
    return false;
}