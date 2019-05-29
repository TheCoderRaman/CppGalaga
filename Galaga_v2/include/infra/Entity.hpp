#ifndef ENTITY_HPP
#define ENTITY_HPP

#include <functional>
#include "infra/Util.hpp"
#include "infra/Collider.hpp"
#include "infra/Message.hpp"
#include "infra/media_layer/Renderer.hpp"
#include "infra/Scene.hpp"

namespace ol
{

    template <typename T>
    class Entity
    {

        static_assert(std::is_base_of<Scene, T>::value, "T must inherit from Scene");

        public:

            Entity(T& scene, int zOrder) : scene_(scene), isDestroyed_(false),
                                           zOrder_(zOrder), newZOrder_(zOrder),
                                           entityID_(nextEntityID_++)
            {
                isHitable_ = true;
                updateAction_ = [] () { };
                nextUpdateAction_ = [] () { };
                hasUpdateActionChanged_ = false;
            }

            virtual ~Entity() { }

            virtual T& getScene() final
            {
                return scene_;
            }

            virtual int getId() final
            {
                return entityID_;
            }

            virtual int getZOrder()
            {
                return zOrder_;
            }

            virtual void changeZOrder(int zOrder)
            {
                newZOrder_ = zOrder;
            }

            virtual bool isZOrderChanged()
            {
                return zOrder_ != newZOrder_;
            }

            virtual bool commitZOrderChange()
            {
                zOrder_ = newZOrder_;
            }

            virtual bool isDestroyed()
            {
                return isDestroyed_;
            }

            virtual void destroy()
            {
                isDestroyed_ = true;
            }

            float getFrame()
            {
                return frame_;
            }

            void setFrame(float frame)
            {
                frame_ = frame;
            }

            virtual const Vec2& getPosition()
            {
                return position_;
            }

            virtual void setPosition(Vec2 position)
            {
                position_ = position;
            }

            virtual const Vec2& getSize()
            {
                return size_;
            }

            virtual void setSize(Vec2 size)
            {
                size_ = size;
            }

            virtual Collider getCollider()
            {
                return Collider(position_ - 0.5 * size_, size_);
            }

            virtual bool isHitable()
            {
                return isHitable_;
            }

            virtual void setHitable(bool isHitable)
            {
                isHitable_ = isHitable;
            }

            virtual void hit(Collider& collider)
            {
            }

            virtual void update()
            {
                if (hasUpdateActionChanged_)
                {
                    updateAction_ = nextUpdateAction_;
                    hasUpdateActionChanged_ = false;
                }
                updateAction_();
            }

            virtual void updateDebug()
            {
            }

            virtual void draw(Renderer& renderer)
            {
                drawSprite(renderer, sprite_, position_, frame_);
            }

            virtual void drawCollider(Renderer& renderer)
            {
                Collider c = getCollider();
                renderer.drawRectangle(c.position, c.size, Color::WHITE);
            }

            virtual void drawSprite(Renderer& renderer, Sprite sprite, Vec2 position, float frame)
            {
                Vec2 srcPosition(static_cast<int>(frame) * size_.x, 0);
                renderer.drawSprite(sprite, position - 0.5 * size_, srcPosition, size_);
            }

            virtual void onMessageReceived(Message message)
            {
            }

            virtual void doNothing()
            {
                setUpdateAction([] () { });
            }

        protected:

            virtual void checkCollisions()
            {
            }

            virtual void setUpdateAction(std::function<void ()> updateAction)
            {
                hasUpdateActionChanged_ = true;
                nextUpdateAction_ = updateAction;
            }

        protected:
            T& scene_;
            bool isDestroyed_;

            float frame_;
            Sprite sprite_;
            Vec2 position_;
            Vec2 size_;
            bool isHitable_;

            std::function<void ()> updateAction_;
            std::function<void ()> nextUpdateAction_;
            bool hasUpdateActionChanged_;

        private:
            int zOrder_;
            int newZOrder_;

            const int entityID_;
            static int nextEntityID_;

    };

    template <typename T>
    int Entity<T>::nextEntityID_ = 0;

}

#endif // ENTITY_HPP

