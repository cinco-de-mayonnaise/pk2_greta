//#########################
//Pekka Kana 2
//Copyright (c) 2003 Janne Kivilahti
//#########################
#pragma once

#include "engine/PFile.hpp"
#include "engine/types.hpp"

#include <list>
#include <vector>
#include <string>
#include <array>

#include "engine/PJson.hpp"
#include "spriteclass_spr.hpp"

class PrototypeClass;

void Prototype_ClearAll();
PrototypeClass* Prototype_Load(const std::string& filename);

//Classes used in game
class SpriteAnimation{
public:
    SpriteAnimation(){};
    SpriteAnimation(const LegacySprAnimation& anim){
        this->loop = anim.loop;
        this->sequence = std::vector<int>(anim.sequence, anim.sequence + anim.frames);
    }
    bool loop = false;
    std::vector<int> sequence;

    friend void to_json(nlohmann::json& j, const SpriteAnimation& a);
    friend void from_json(const nlohmann::json&j, SpriteAnimation& a);
};

class PrototypeClass{
    public:

    std::string version = "2.0";

    std::string filename;
    std::string picture_filename;
    
    int     type = TYPE_NOTHING;

    std::array<std::string, SPRITE_SOUNDS_NUMBER> sound_files = {""};
    std::array<int, SPRITE_SOUNDS_NUMBER> sounds = {-1};

    //int     framet[SPRITE_MAX_FRAMEJA] = {0};
    std::vector<int>frames;
    std::vector<int>frames_mirror;
    //int     framet_peilikuva[SPRITE_MAX_FRAMEJA] = {0};
    int      frames_number            = 0;

    std::array<SpriteAnimation, SPRITE_ANIMATIONS_NUMBER> animations;
    //PK2SPRITE_ANIMAATIO animaatiot[SPRITE_MAX_ANIMAATIOITA] = {};
    //u8      animations_number       = 0;
    int      frame_rate         = 0;
    int     picture_frame_x             = 0;
    int     picture_frame_y             = 0;
    int     picture_frame_width  = 0;
    int     picture_frame_height = 0;

    std::string name;
    int     width    = 0;
    int     height   = 0;
    double  weight    = 0;

    bool    enemy     = false;
    int     energy        = 0;
    int     damage        = 0;
    int      damage_type = DAMAGE_IMPACT;
    int      immunity_type        = DAMAGE_NONE;
    int     score        = 0;

    std::vector<int> AI_v;
    int first_ai()const{
        return AI_v.empty()? 0 : AI_v[0];
    }

    int      max_jump    = 0;
    double  max_speed   = 3;
    int     charge_time  = 0;
    u8      color         = COLOR_NORMAL;
    bool    is_wall         = false;
    int     how_destroyed = FX_DESTRUCT_ANIMAATIO;
    bool    can_open_locks        = false;
    bool    vibrates      = false;
    int      bonuses_number = 1;
    int     attack1_time = 60;
    int     attack2_time = 60;

    int     parallax_type = 0;

    std::string transformation_sprite;
    std::string bonus_sprite;
    std::string ammo1_sprite;
    std::string ammo2_sprite;


    PrototypeClass* transformation     = nullptr;
    PrototypeClass* bonus      = nullptr;
    PrototypeClass* ammo1     = nullptr;
    PrototypeClass* ammo2     = nullptr;

    bool    makes_sounds = true;
    int     sound_frequency      = 22050;
    bool    random_sound_frequency    = true;

    bool    is_wall_up       = true;
    bool    is_wall_down       = true;
    bool    is_wall_right   = true;
    bool    is_wall_left = true;

    u8      effect       = EFFECT_NONE;
    bool    is_transparent       = false;
    int     projectile_charge_time    = 0;
    bool    can_glide    = false;
    bool    boss         = false;
    bool    bonus_always = false;
    bool    can_swim     = false;

    /**
     * @brief 
     * Field for the purpose of counting.
     * If Episode->ignore_collectable==false, this field will be always false.
     */
    bool    big_apple     = false;   //

    /**
     * @brief 
     * If true a sprite will be active even far from the camera
     */
    bool    always_active = false;   // 

    PrototypeClass();
    ~PrototypeClass();

    /**
     * @brief 
     * Load sprite prototype
     */
    int     LoadPrototype(PFile::Path path, bool legacy_spr);
    /**
     * @brief 
     * Load sprite assets like texture, sounds, etc.
     */
    int     LoadAssets(PFile::Path path);

    
    int     Draw(int x, int y, int frame);
    bool    HasAI(int AI)const;

    void    SetProto10(PrototypeClass10 &proto);
    void    SetProto11(PrototypeClass11 &proto);
    void    SetProto12(PrototypeClass12 &proto);
    void    SetProto13(PrototypeClass13 &proto);

    void    SetProto20(const nlohmann::json& j);
};
class SpriteClass{
    public:

    bool    active       = false;           // if the sprite is acting
    int     player          = 0;               // 0 = isn't player, 1 = is player
    PrototypeClass *prototype   = nullptr;         // the sprite prototype
    bool    removed          = true;            // the sprite was removed
    double  orig_x           = 0;               // original x location
    double  orig_y           = 0;               // original y location
    double  x                = 0;               // sprite x location
    double  y                = 0;               // sprite y location
    double  a                = 0;               // horizontal speed
    double  b                = 0;               // vertical speed
    bool    flip_x           = false;           // if it is flipped horizontally
    bool    flip_y           = false;           // if it is flipped vertically
    int     jump_timer       = 0;               // jump times: = 0 not jumping; > 0 jumping; < 0 falling
    bool    ylos             = true;            // can sprite move up now?
    bool    alas             = true;            // can sprite move down now?
    bool    oikealle         = true;            // can sprite move right now?
    bool    vasemmalle       = true;            // can sprite move left now?
    bool    reuna_vasemmalla = false;           // is there a pit on the left side of the sprite?
    bool    reuna_oikealla   = false;           // is there a pit on the right side of the sprite?
    int     energy          = 0;               // the sprite energy
    SpriteClass *emosprite   = nullptr;         // the sprite's parent
    double  weight           = 0;               // sprite weight
    double  kytkinpaino      = 0;               // sprite weight + weight above him (why it doesn't work?)
    bool    crouched         = false;           // if the sprite is crouched
    int     damage_timer     = 0;               // damage timer
    int     invisible_timer  = 0;               // invisibility timer
    int     super_mode_timer = 0;               // super mode timer
    int     charging_timer   = 0;               // charging time for the attacks
    int     attack1_timer    = 0;               // timer after attack 1
    int     attack2_timer    = 0;               // timer after attack 2
    bool    in_water          = false;           // if the sprite is inside water
    bool    hidden         = false;           // if the sprite is hidden
    double  initial_weight   = 0;               // sprite's original weight - the same as that of the prototype
    int     saatu_vahinko    = 0;               // damage taken
    u8      saatu_vahinko_tyyppi = DAMAGE_NONE; // damage taken type (e.g. snow).
    bool    enemy       = false;           // if it is a enemy
    PrototypeClass* ammo1   = nullptr;         // projectile 1
    PrototypeClass* ammo2   = nullptr;         // projectile 2

    int     seen_player_x    = -1;              // where the player was last seen x
    int     seen_player_y    = -1;              // where the player was last seen y

    int     action_timer     = 0;               // timer for some AI actions. vary from 1 to 32000
 
    int      animation_index  = ANIMATION_IDLE;  // animation index
    unsigned int current_sequence = 0;               // current sequence
    int      frame_timer      = 0;               // frame times
    int     mutation_timer   = 0;               // the mutation timer

    
    SpriteClass();
    SpriteClass(PrototypeClass *prototype, int player, double x, double y);
    ~SpriteClass();

    int  Draw(int kamera_x, int kamera_y);   // animate and draw the sprite
    int  Animaatio(int anim_i, bool nollaus);  // set sprite animation
    int  Animoi();                             // animate the sprite
    void HandleEffects();                      // create sprite effects
    bool HasAI(int AI)const{
        return prototype->HasAI(AI);
    }; // if the sprite has a AI


    //AI_Functions
    int AI_Kana();
    int AI_Bonus();
    int AI_Egg();
    int AI_Egg2();
    int AI_Ammus();
    int AI_Jumper();
    int AI_Sammakko1();
    int AI_Sammakko2();
    int AI_Basic();
    int AI_Kaantyy_Esteesta_Hori();
    int AI_Kaantyy_Esteesta_Vert();
    int AI_Varoo_Kuoppaa();
    int AI_Random_Kaantyminen();
    int AI_Random_Suunnanvaihto_Hori();
    int AI_Random_Hyppy();
    int AI_Random_Liikahdus_Vert_Hori();
    int AI_Seuraa_Pelaajaa(SpriteClass &player);
    int AI_Seuraa_Pelaajaa_Jos_Nakee(SpriteClass &player);
    int AI_Seuraa_Pelaajaa_Jos_Nakee_Vert_Hori(SpriteClass &player);
    int AI_Seuraa_Pelaajaa_Vert_Hori(SpriteClass &player);
    int AI_Jahtaa_Pelaajaa(SpriteClass &player);
    int AI_Pakenee_Pelaajaa_Jos_Nakee(SpriteClass &player);
    int AI_Change_When_Energy_Under_2(PrototypeClass *transformation);
    int AI_Change_When_Energy_Over_1(PrototypeClass *transformation);
    int AI_Muutos_Ajastin(PrototypeClass *transformation);
    int AI_Muutos_Jos_Osuttu(PrototypeClass *transformation);
    int AI_Attack_1_Jos_Osuttu();
    int AI_Attack_2_Jos_Osuttu();
    int AI_Attack_1_Nonstop();
    int AI_Attack_2_Nonstop();
    int AI_Attack_1_if_Player_in_Front(SpriteClass &player);
    int AI_Attack_2_if_Player_in_Front(SpriteClass &player);
    int AI_Attack_1_if_Player_Bellow(SpriteClass &player);
    int AI_NonStop();
    int AI_Hyppy_Jos_Pelaaja_Ylapuolella(SpriteClass &player);
    int AI_Pommi();
    int AI_Damaged_by_Water();
    int AI_Kill_Everyone();
    int AI_Kitka_Vaikuttaa();
    int AI_Piiloutuu();
    int AI_Palaa_Alkuun_X();
    int AI_Palaa_Alkuun_Y();
    int AI_Kaantyy_Jos_Osuttu();
    int AI_Tippuu_Tarinasta(int tarina);
    int AI_Liikkuu_X(double liike);
    int AI_Liikkuu_Y(double liike);
    int AI_Tippuu_Jos_Kytkin_Painettu(int kytkin);
    int AI_Liikkuu_Jos_Kytkin_Painettu(int kytkin, int ak, int bk);
    int AI_Teleportti(std::list<SpriteClass*> spritet, SpriteClass &player);
    int AI_Kiipeilija();
    int AI_Kiipeilija2();
    bool AI_Info(SpriteClass &player);
    int AI_Tuhoutuu_Jos_Emo_Tuhoutuu();

    int AI_Destructed_Next_To_Player(SpriteClass &player);

    int Animation_Perus();
    int Animation_Kana();
    int Animation_Bonus();
    int Animation_Egg();
    int Animation_Ammus();
};
