#ifndef STAGE_HPP
#define STAGE_HPP

#include <memory>
#include <list>
#include <iterator>
#include "infra/Path.hpp"
#include "infra/Hud.hpp"
#include "infra/Scene.hpp"
#include "infra/Entity.hpp"

namespace ol
{
    class GameController;
    class FighterExplosion;
    class EnemyExplosion;

    class Stage : public Scene
    {
        public:
            Stage(SceneManager& sceneManager);
            virtual ~Stage();

            void onEnter();
            void onExit();

            GameController* getGameController();

            void update();
            void draw(Renderer& renderer);

            void broadcastMessage(Message message);

            template <class T>
            T* spawn()
            {
                std::shared_ptr<T> t = std::make_shared<T>(*this);
                entitiesAdd_.push_back(t);
                return &*t;
            }

            bool isEntityAlive(int id)
            {
                for (auto entity : entities_)
                {
                    if (entity->getId() == id && !entity->isDestroyed())
                    {
                        return true;
                    }
                }
                for (auto entity : entitiesAdd_)
                {
                    if (entity->getId() == id && !entity->isDestroyed())
                    {
                        return true;
                    }
                }
                return false;
            }

            template <class T>
            T* getEntityById(int id)
            {
                for (auto entity : entities_)
                {
                    if (entity->getId() == id)
                    {
                        return static_cast<T*>(entity.get());
                    }
                }
                for (auto entity : entitiesAdd_)
                {
                    if (entity->getId() == id)
                    {
                        return static_cast<T*>(entity.get());
                    }
                }
                return nullptr;
                /*
                std::list<std::shared_ptr<Entity<Stage>>>::iterator it;
                it = find(entities_.begin(), entities_.end(),
                               [id] (std::shared_ptr<Entity<Stage>> const& entity)
                               {
                                   return entity->getId() == id;
                               });
                return static_cast<T*>((*it).get());
                */
            }

            template <class T>
            T* checkCollision(Entity<Stage>* source)
            {
                if (source == nullptr || source->isDestroyed() || !source->isHitable())
                {
                    return nullptr;
                }

                for (auto entity : entities_)
                {
                    if (&*entity == source || typeid(*entity) != typeid(T)
                        || entity->isDestroyed() || !entity->isHitable())
                    {
                        continue;
                    }

                    if (source->getCollider().collides(entity->getCollider()))
                    {
                        return dynamic_cast<T*>(&*entity);
                    }
                }
                return nullptr;
            }

        protected:

        private:
            GameController* gameController_;

            std::list<std::shared_ptr<Entity<Stage>>> entities_;
            std::list<std::shared_ptr<Entity<Stage>>> entitiesAdd_;
    };

}

#endif // STAGE_HPP
