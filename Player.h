#ifndef PLAYER_H
#define PLAYER_H

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include "GameObject.h"

enum class PlayerState { IDLE, MOVE_LEFT, MOVE_RIGHT, JUMP, FALL };

class Player: public GameObject{
    private:
        int moveSpeed;
        double ySpeed;
        PlayerState playerState;

		int jumping;

        double health;
        double maxHealth;

        bool gotTP;
	bool damaged;
	Sprite * sprite_damaged;
    public:
        //Player(SDL_Renderer *ren, const char *file):GameObject(ren, file){};
        Player();
        PlayerState GetPlayerState();
        int GetMovementSpeed();
        int GetYSpeed();
		int GetJumping();
        bool GotTP();
	bool GetDamaged();

        double GetHealth();
        double GetMaxHealth();

        void SetMovementSpeed(int speed);
        void SetYSpeed(int speed);
		void SetJumping(int jump);
        void SetPlayerState(PlayerState state);
        void SetHealth(double h);
        void SetMaxHealth(double h);
        void GotTP(bool b);
	void SetDamaged(bool d);

        void ChangeHealth(double i);
	
	Sprite * GetDamagedSprite();
	void SetDamagedSprite(SDL_Renderer * ren, const char *file);
	void RenderDamaged(SDL_Renderer * ren, double angle, SDL_Point* center, SDL_RendererFlip flip);

        //overrides
        void Update() override;
        void DoCollisionResponse(std::shared_ptr<GameObject> objCollidedWith) override;
        void SetIdle() override;
        std::string PrintObjType() override;

};

#endif
