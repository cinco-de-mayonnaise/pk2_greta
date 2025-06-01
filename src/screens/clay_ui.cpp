//#########################
//Pekka Kana 2
//Copyright (c) 2003 Janne Kivilahti
//#########################
/**
 * @brief 
 * New UI layout engine(Clay) util ported by AbdullahTrees to replace hardcoded menu layouts.
 */
#define CLAY_IMPLEMENTATION
#include "clay_ui.hpp"

#include "engine/PDraw.hpp"
#include "system.hpp"
#include "engine/PLog.hpp"

namespace ClayUI {

    Clay_String MakeClayString(const char *str, bool copy)
    {
        Clay_String cs = {.length = std::strlen(str), .chars = str, };

        return cs;
    }

    Clay_Dimensions Clay_MeasureTextFunction_Abstraction(Clay_StringSlice text, Clay_TextElementConfig *config, void *userData)
    {
        std::string str(text.chars, text.length);
        
        auto sz = PDraw::font_get_text_size(config->fontId, str);
        auto ret = (Clay_Dimensions){.width = sz.first, .height = sz.second};
        //PLog::Write(PLog::DEBUG, "ClayMeasureText", "str is %s, h: %f w: %f", str.c_str(), ret.height, ret.width);
        return ret;
    }

    void HandleClayErrors(Clay_ErrorData errorData) {
        // See the Clay_ErrorData struct for more information
        PLog::Write(PLog::ERR, "Clay", errorData.errorText.chars);
        // switch(errorData.errorType) {
        //     // etc
        // }
    }

    void Update() {

        // hi!!! this might be important in the future 
        // PRender::get_window_size(&w, &h);

        Clay_SetLayoutDimensions((Clay_Dimensions) { 640, 480 });
        // Optional: Update internal pointer position for handling mouseover / click / touch events - needed for scrolling & debug tools
        Clay_SetPointerState((Clay_Vector2) { PInput::mouse_x, PInput::mouse_y }, Clicked());
        // Optional: Update internal pointer position for handling mouseover / click / touch events - needed for scrolling and debug tools
        Clay_UpdateScrollContainers(
            true, // Enable drag scrolling
            {0, -0.01}, // Clay_Vector2 scrollwheel / trackpad scroll x and y delta this frame
            1.0f/Piste::get_fps() // Time since last frame in seconds as a float e.g. 8ms is 0.008f
        );
    }

    void Init() {
        uint32_t minMemoryRequired = Clay_MinMemorySize();
        Clay_Arena clayMemory = Clay_CreateArenaWithCapacityAndMemory(minMemoryRequired, malloc(minMemoryRequired));

        Clay_Initialize(clayMemory, (Clay_Dimensions){640, 480}, (Clay_ErrorHandler){HandleClayErrors});

        // PDraw Init required
        Clay_SetMeasureTextFunction(Clay_MeasureTextFunction_Abstraction, NULL);
    }

    void Draw_ClayLayout(Clay_RenderCommandArray to_draw) {
        for (int i = 0; i < to_draw.length; i++) {
            Clay_RenderCommand *renderCommand = &to_draw.internalArray[i];
            Clay_BoundingBox boundingBox = renderCommand->boundingBox;

            switch (renderCommand->commandType) {
                case CLAY_RENDER_COMMAND_TYPE_RECTANGLE: {
                    //DrawRectangle( renderCommand->boundingBox, renderCommand->renderData.rectangle.backgroundColor);
                    uint16_t left = boundingBox.x,
                                top = boundingBox.y,
                                right = boundingBox.width + boundingBox.x, 
                                bottom = boundingBox.height + boundingBox.y;
                    
                    //u8 color = renderCommand->renderData.rectangle.backgroundColor;
                    PLog::Write(PLog::INFO, "ClayRenderCommandArguments", "Screen Fill: %d %d %d %d", left, top, right, bottom);
                    PDraw::screen_fill(left, top, right, bottom, 0);
                    //globalcolor++; 
                    //PDraw::font_write(fontti3,std::to_string(globalcolor).c_str(),0,0);
                } break;
                case CLAY_RENDER_COMMAND_TYPE_TEXT: {
                    Clay_TextRenderData *config = &renderCommand->renderData.text;
                    std::string clonestr(config->stringContents.chars, config->stringContents.length);
                    
                    PDraw::font_write(renderCommand->renderData.text.fontId, clonestr.c_str(), boundingBox.x, boundingBox.y);
                    //PLog::Write(PLog::INFO, "ClayRenderCommandArguments", "Font Write: %s, %d %d", clonestr.c_str(), (int)boundingBox.x, (int)boundingBox.y);
                } break;
                case CLAY_RENDER_COMMAND_TYPE_SCISSOR_START: {
                    //PLog::Write("I'm about to cliiiiiiip on: %.1f %.1f %.1f %.1f", boundingBox.x, boundingBox.y, boundingBox.width, boundingBox.height);
                    PDraw::set_mask(boundingBox.x, boundingBox.y, boundingBox.width, boundingBox.height);
                } break;
                case CLAY_RENDER_COMMAND_TYPE_SCISSOR_END: {
                    //PLog::Write("Whew, done clipping...");
                    PDraw::reset_mask();
                } break;
                case CLAY_RENDER_COMMAND_TYPE_BORDER:
                case CLAY_RENDER_COMMAND_TYPE_IMAGE:
                case CLAY_RENDER_COMMAND_TYPE_NONE:
                case CLAY_RENDER_COMMAND_TYPE_CUSTOM:   // not handled yet
                break;
            }
        }
    }
}
