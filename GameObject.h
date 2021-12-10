#ifndef GAMEOBJECT_H
#define GAMEOBJECT_H

#include <SDL2/SDL.h>
#include <memory>
#include "Sprite.h"
enum class ObjType { None, Player, Pushable, Health, Enemy, TP, Checkout, Box };

class GameObject{
    private:
        SDL_Renderer *renderer;
        Sprite *sprite;
        SDL_Rect boxCollider;
        int alpha;
        //variables for when one object is on top of another
        bool onTop;
        std::shared_ptr<GameObject> onTopOf;

        SDL_Rect* screen_rect;//Background change

    protected:
        ObjType objType;
        
    public:
        GameObject();
        virtual ~GameObject();

        void Init(SDL_Renderer *ren, const char *file,SDL_Rect* camera);//Background change
        virtual void Render();
        void Render(double angle, SDL_Point *center, SDL_RendererFlip flip);
        void RenderBoxCollider();

        Sprite* GetSprite();
        ObjType GetType();
        SDL_Rect GetBoxCollider();
        int GetScreenRecX();//Background change
        SDL_Renderer * GetRenderer(){ return renderer; };
        int GetAlpha();
        bool GetOnTop();
        std::shared_ptr<GameObject> GetOnTopOf();

        void SetBoxCollider(int x, int y, int w, int h);
        void SetBoxColliderPos(int x, int y);
        void SetBoxCollider(SDL_Rect *r);
        void SetAlpha(int a);
        void SetOnTopOf(std::shared_ptr<GameObject> below);
        void SetOnTop(bool x);
        void SetType(ObjType type);

        void ChangeAlpha(int var);

        void MoveX(int i);
        void MoveY(int i);

        //to be overridden by child classes
        virtual void Update();
        virtual void DoCollisionResponse(std::shared_ptr<GameObject> objCollidedWith);
        virtual void SetIdle();
        virtual std::string PrintObjType();
};

#endif
