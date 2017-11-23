//
//  PineEffect.h
//  PineEntertainment
//
//  Created by Do Thanh Nghia on 1/7/16.
//
//

#ifndef PineEffect_h
#define PineEffect_h
class PineEffect
{
    
};


class PineText
{
   
    char* _contents;
    int _word_num;
    ;
public:
    int* _indexWord;
    int GetWordNum();
    const char* GetContents();
    static char _buf_content[10240];
    PineText();
    void InitWithFormat(const char* text, ...);
    ~PineText();
       
};

class PineTextTypeWordEffect: public PineEffect
{
    PineText* _content;
    float _char_space;
    float _word_space;
    float _line_space;
    int _frame_update;
    int _frame_counter;
    unsigned int _color_mask;
public:
    void SetColorMask(unsigned int color);
    void  SetFrameChangeWord(int frame);
    void  SetCharSpacing(float s);
    float GetCharSpacing();
    void  SetWordSpacing(float s);
    float GetWordSpacing();
    void  SetLineSpacing(float s);
    float GetLineSpacing();
    int spriteid;
    int _index_type;
    void InitWithFormat(int sprite_id,const char* text, ...);
    void Update();
    void Render(float x, float y, int width, int anchor);
    bool IsCurrentEffectStop();
    PineTextTypeWordEffect();
    ~PineTextTypeWordEffect();
    int _opacity;
    
};

#endif /* PineEffect_h */
