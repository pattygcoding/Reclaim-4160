/*
    Nikita Tran
    CPSC 4160 2D Game Engine Construction
    Fall 2020
*/

#include "Player.h"
#include "GameEngine.h"

#include <string>
#include <SDL2/SDL_image.h>
#include <iostream>

Player::Player(){
    objType = ObjType::Player;
    playerState = PlayerState::IDLE;
    ySpeed = 0;
    moveSpeed = 5;

    jumping = 0;

    health = 100;
    maxHealth = 100;

    gotTP = 0;
    damaged = 0;
};

GameEngine* game = GameEngine::get_instance();

PlayerState Player::GetPlayerState(){return playerState;}
int Player::GetMovementSpeed(){return moveSpeed;}
int Player::GetYSpeed(){return ySpeed;}
int Player::GetJumping(){return jumping;}
double Player::GetHealth(){return health;}
double Player::GetMaxHealth(){return maxHealth;}
bool Player::GotTP(){ return gotTP; }
bool Player::GetDamaged(){return damaged;}

/*
    state = one of the valid states that Player can have. Valid states (defined in header file) are IDLE, MOVE_LEFT, MOVE_RIGHT, JUMP, FALL
    (jump state can be set but it doesn't do anything yet in this build)
*/
void Player::SetPlayerState(PlayerState state){playerState = state;}
void Player::SetMovementSpeed(int speed){moveSpeed = speed;}
void Player::SetYSpeed(int speed){ySpeed = speed;}
void Player::SetJumping(int jump){jumping = jump;}
void Player::SetHealth(double h){health = h;}
void Player::SetMaxHealth(double h){maxHealth = h;}
void Player::GotTP(bool b){ gotTP = b; }
void Player::SetDamaged(bool d){damaged = d;}

void Player::ChangeHealth(double i){ 
    health += i;
    if (health < 0) health = 0;
    if (health > maxHealth) health = maxHealth;
}

//Updates player's position and sprite animation frame depending on player's state
void Player::Update(){
    int moveAnimYOffset = GetSprite()->GetSrcH(); //used to set sprite's src_rect to the next row of sprites in the sprite sheet
    int moveAnimTotalFrames = 4;
    int moveAnimStartFrame = 0; //which animation frame you want to start at

    int animSpeed = 200;

    int idleAnimTotalFrames = 4;
    int idleAnimStartFrame = 0;

    //link collider to player pos
    SetBoxColliderPos(GetSprite()->GetX(), GetSprite()->GetY());

    //Keep player from falling through floor, bring them back to floor
    if(GetSprite()->GetY() + GetSprite()->GetH() > game->GetScreenHeight() - game->GetFloorY()){
        ySpeed = 0;
        GetSprite()->SetY(game->GetScreenHeight() - GetSprite()->GetH() - game->GetFloorY());
        SetPlayerState(PlayerState::IDLE);
    }
    else if(GetOnTop() == false && GetSprite()->GetY() + GetSprite()->GetH() < game->GetScreenHeight() - game->GetFloorY() && playerState != PlayerState::MOVE_RIGHT && playerState != PlayerState::MOVE_LEFT){
        SetPlayerState(PlayerState::FALL);
    }

    switch(playerState){
        case PlayerState::IDLE: {
            if(gotTP){
 		GetSprite()->SetSrcY(moveAnimYOffset * 2);
		sprite_damaged->SetSrcY(moveAnimYOffset * 2);
	    }
            else {
		GetSprite()->SetSrcY(moveAnimYOffset * 0);
		sprite_damaged->SetSrcY(moveAnimYOffset * 0);
	    }

            GetSprite()->UpdateFrame(animSpeed, idleAnimStartFrame, idleAnimTotalFrames);
            break;
        }

        case PlayerState::MOVE_LEFT: {
            if(gotTP) { 
		GetSprite()->SetSrcY(moveAnimYOffset * 3);
		sprite_damaged->SetSrcY(moveAnimYOffset * 3);
	    }
            else {
		GetSprite()->SetSrcY(moveAnimYOffset * 1);
		sprite_damaged->SetSrcY(moveAnimYOffset * 1);
	    }
            GetSprite()->UpdateFrame(animSpeed, moveAnimStartFrame, moveAnimTotalFrames);

            MoveX(moveSpeed * -1);
	    
            GetSprite()->SetFlip(SDL_FLIP_HORIZONTAL);
            break;
        }

        case PlayerState::MOVE_RIGHT: {
            if(gotTP) { 
		GetSprite()->SetSrcY(moveAnimYOffset * 3);
		sprite_damaged->SetSrcY(moveAnimYOffset * 3);
	    }
            else {
		GetSprite()->SetSrcY(moveAnimYOffset * 1);
		sprite_damaged->SetSrcY(moveAnimYOffset * 1);
	    }
            GetSprite()->UpdateFrame(animSpeed, moveAnimStartFrame, moveAnimTotalFrames);

            MoveX(moveSpeed);
	    
            GetSprite()->SetFlip(SDL_FLIP_NONE);
            break;
        }

        case PlayerState::JUMP: {
	    if(jumping < 1) {
	        //std::cout << "Jumping";
	        jumping++;
            	ySpeed = -9.0;
		if(gotTP) { 
		    GetSprite()->SetSrcY(moveAnimYOffset * 3);
		    sprite_damaged->SetSrcY(moveAnimYOffset * 3);
	    	}
            	else {
		    GetSprite()->SetSrcY(moveAnimYOffset * 1);
		    sprite_damaged->SetSrcY(moveAnimYOffset * 1);
	    	}
            	GetSprite()->UpdateFrame(animSpeed, moveAnimStartFrame, moveAnimTotalFrames);

	        if(GetSprite()->GetY() > 0) {
	    	    MoveY(ySpeed);
	        }
	    }

            break;
        }
        case PlayerState::FALL: {
	    if(gotTP) { 
		GetSprite()->SetSrcY(moveAnimYOffset * 2);
		sprite_damaged->SetSrcY(moveAnimYOffset * 2);
	    }
            else {
		GetSprite()->SetSrcY(moveAnimYOffset * 0);
		sprite_damaged->SetSrcY(moveAnimYOffset * 0);
	    }
            GetSprite()->UpdateFrame(animSpeed, idleAnimStartFrame, idleAnimTotalFrames);
            MoveY(ySpeed);
            ySpeed += 0.25;
            if(GetYSpeed() < 0){ SetOnTopOf(NULL);}

            break;
        }
    }

    //adhere to screen bounds
    if(GetSprite()->GetX() < 0){GetSprite()->SetX(0);}

    if(GetSprite()->GetX() > game->getBgWidth()- GetSprite()->GetW()){//Background change
        GetSprite()->SetX(game->getBgWidth() - GetSprite()->GetW());//Background change
    }

    return;
}

void Player::DoCollisionResponse(std::shared_ptr<GameObject> objCollidedWith){ 
    //If player y velocity is positive (meaning they are moving down) and they are now colliding with 
    //an object's top, set the y velocity to 0
    if(GetYSpeed() > 0 && GetOnTopOf() == NULL){
        SetOnTopOf(objCollidedWith);
        SetPlayerState(PlayerState::IDLE);
    }

    switch(objCollidedWith->GetType()){
        case ObjType::Health: {
            health += 10;
            if(health > maxHealth) health = maxHealth;
            break;
        }

        default: {
            break;
        }
    }
}

Sprite* Player::GetDamagedSprite() {
    return sprite_damaged;
}

void Player::SetDamagedSprite(SDL_Renderer * ren, const char *file) {
    sprite_damaged = new Sprite(ren, file);
}

void Player::RenderDamaged(SDL_Renderer * ren, double angle, SDL_Point* center, SDL_RendererFlip flip){
    SDL_Rect temp;//Background change
    temp.x = GetSprite()->GetScreenRect()->x - GetScreenRecX();//Background change
    temp.y = GetSprite()->GetScreenRect()->y;//Background change
    temp.w = GetSprite()->GetScreenRect()->w;//Background change
    temp.h = GetSprite()->GetScreenRect()->h;//Background change
    SDL_RenderCopyEx(ren, sprite_damaged->GetTexture(), sprite_damaged->GetSrcRect(),
			 &temp, angle, center, flip);//Background change
}

void Player::SetIdle(){
    SetPlayerState(PlayerState::IDLE);
}

std::string Player::PrintObjType(){
    return "Player";
}
