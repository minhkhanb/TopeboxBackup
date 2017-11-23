//
//  PineEffect.cpp
//  PineEntertainment
//
//  Created by Do Thanh Nghia on 1/7/16.
//
//

#include "pch.h"
#include "PineSocial.h"
#include "../GameCore/CGame.h"
#include "../GameCore/Os_Functions.h"
#include <stdlib.h>

char PineText::_buf_content[10240];
PineText::PineText()
{
    _contents = NULL;
    _indexWord = NULL;
    _word_num = 0;
    
}
void PineText::InitWithFormat(const char* text, ...)
{
    SAFE_DEL_ARRAY(_contents);
    SAFE_DEL_ARRAY(_indexWord);
    va_list	argptr;
    va_start(argptr, text);
    vsprintf(_buf_content, text, argptr);
    va_end(argptr);
    _contents = new char[strlen(_buf_content)+1];
    sprintf(_contents,"%s",_buf_content);
    //count word
    for(int i= 0; i < strlen(_contents); i++)
    {
        if(_contents[i] == ' ')
        {
            _word_num++;
        }
    }
    if(strlen(_contents) >= 1)
    {
        _word_num++;
    }
    if(_word_num > 0)
    {
        
        _indexWord = new int[_word_num];
        _word_num = 0;
        for(int i = 0; i< strlen(_contents); i++)
        {
            if(_contents[i] == ' ')
            {
                _indexWord[_word_num] = i+1;
                _word_num++;
            }
        }
        _indexWord[_word_num] = strlen(_contents);
        _word_num++;
    }
    
    //printf("\n_word_num = %d",_word_num);
    //printf("\n_word_num = %d",_word_num);
    //printf("\n_word_num = %d",_word_num);
    
}
int PineText::GetWordNum()
{
    return _word_num;
}
const char* PineText::GetContents()
{
    return _contents;
}
PineText::~PineText()
{
    SAFE_DEL_ARRAY(_contents);
    SAFE_DEL_ARRAY(_indexWord);
}


/////Effect
void PineTextTypeWordEffect::InitWithFormat(int sprite_id,const char* text, ...)
{
    SAFE_DEL(_content);
    _content = new PineText();
    va_list	argptr;
    va_start(argptr, text);
    vsprintf(PineText::_buf_content, text, argptr);
    va_end(argptr);
    _content->InitWithFormat(PineText::_buf_content);
    
    spriteid = sprite_id;
    _index_type = 0;
    _char_space = GET_SPRITE(spriteid)->_char_spacing;
    _word_space = GET_SPRITE(spriteid)->_word_spacing;
    _line_space = GET_SPRITE(spriteid)->_line_spacing;
    _frame_counter = 0;
    _opacity = 0;
}
void PineTextTypeWordEffect::Update()
{
    
    _opacity += 100/_frame_update;
    if(_opacity >= 100)
    {
        if(_index_type < _content->GetWordNum()-1)
        {
            _index_type++;
            _opacity = 0;
        }
        else
        {
            _opacity = 100;
        }
    }
    
    
}
void PineTextTypeWordEffect::Render(float x, float y, int width, int anchor)
{
    if(_content->GetWordNum() > 0)
    {
        int  end = _content->_indexWord[_index_type]-1;
        
        GET_SPRITE(spriteid)->_char_spacing = _char_space;
        GET_SPRITE(spriteid)->_word_spacing = _word_space;
        GET_SPRITE(spriteid)->_line_spacing = _line_space;
        int begin = 0;
        if(_index_type >= 1)
        {
            begin = _content->_indexWord[_index_type-1];
        }
        //printf("\n _content->GetContents() = %s, begin = %d end = %d",_content->GetContents(),begin,len);
        //int opacity = (_frame_counter % _frame_update)*100/_frame_update;
       
       
        
        if(!IsCurrentEffectStop())
        {
            
            G()->SetOpacity(_opacity);
            G()->SetColorMask(_color_mask);
            GET_SPRITE(spriteid)->DrawAnsiNTextPageFormatCustomColorRange(G(), x,  y,begin,end, width , anchor, "%s",_content->GetContents());
            G()->SetOpacity(100);
            
            if(_index_type >= 1)
            {
                //printf("\nbegin = %d",begin);
                G()->SetColorMask(_color_mask);
                GET_SPRITE(spriteid)->DrawAnsiNTextPageFormatCustomColorRange(G(), x,  y,0,begin-2, width , anchor, "%s",_content->GetContents());
            }
        }
        else
        {
            //printf("\n_frame_counter = %d",_frame_counter);
            //printf("\nopacity = %d",opacity);
            G()->SetColorMask(_color_mask);
            GET_SPRITE(spriteid)->DrawAnsiNTextPageFormatCustomColorRange(G(), x,  y,0,end, width , anchor, "%s",_content->GetContents());
        }
        G()->ClearColorMask();

       
    }
}
PineTextTypeWordEffect::PineTextTypeWordEffect()
{
    _content = NULL;
    _char_space = 0;
    _word_space = 0;
    _line_space = 0;
    _frame_update = 1;
    _frame_counter = 1;
}
PineTextTypeWordEffect::~PineTextTypeWordEffect()
{
    SAFE_DEL(_content);
}
void PineTextTypeWordEffect::SetColorMask(unsigned int color)
{
    _color_mask = color;
}
bool PineTextTypeWordEffect::IsCurrentEffectStop()
{
    return (_index_type >= _content->GetWordNum()-1) && (_opacity >= 100);
}

void  PineTextTypeWordEffect::SetCharSpacing(float s)
{
    _char_space = s;
}
float PineTextTypeWordEffect::GetCharSpacing()
{
    return _char_space;
}
void  PineTextTypeWordEffect::SetWordSpacing(float s)
{
    _word_space = s;
}
float PineTextTypeWordEffect::GetWordSpacing()
{
    return _word_space;
}
void  PineTextTypeWordEffect::SetLineSpacing(float s)
{
    _line_space = s;
}
float PineTextTypeWordEffect::GetLineSpacing()
{
    return _line_space;
}
void  PineTextTypeWordEffect::SetFrameChangeWord(int frame)
{
    if(frame <= 0)
    {
        frame = 1;
    }
    _frame_update = frame;
    _frame_counter = 1;
}