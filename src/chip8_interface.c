#include "chip8_interface.h"

// Private constants

enum{ 
    DISPLAY_WIDHT  = 1280,
    DISPLAY_HEIGHT = 720
};

Font font;

// DONE: Refac this file

static void chip8_draw_hardware_component(
    const Vector2 pos, 
    const char* name, 
    const char* value,
    const Color color
){
    const int value_spacing = 65;
    const int name_spacing = 40;
    const int font_size = 16;
    const int font_spacing = 2;

    DrawTextEx(
        font,
        name,
        pos,
        font_size,
        font_spacing,
        color
    );
    DrawTextEx(
        font,
        "=",
        (Vector2){
            pos.x + name_spacing,
            pos.y
        },
        font_size,
        font_spacing,
        color
    );
    DrawTextEx(
        font,
        value,
        (Vector2){
            pos.x + value_spacing,
            pos.y
        },
        font_size,
        font_spacing,
        color
    );
}


static void chip8_draw_display_area(
    const Chip8VM* vm
){
    const int font_spacing = 2;
    const Rectangle chip8_pixel = {
        26,
        28,
        14,
        12,
    };

    const int title_size = 18;
    const int title_x = 10;
    const int title_y = 6;

    const float line_thickness = 2.0;
    const Rectangle external_display_line = {
        2,
        2,
        4,
        4
    };

    DrawTextEx(
        font,
        "Display",
        (Vector2){chip8_pixel.width + title_x,
        chip8_pixel.height - title_y},
        title_size,
        font_spacing,
        GREEN
    );

    for(int i = 0; i < CHIP8_DISPLAY_HEIGHT; i++)
        for (int j = 0; j < CHIP8_DISPLAY_WIDTH; j++)
            DrawRectangle( 
                chip8_pixel.x + j * chip8_pixel.width,
                chip8_pixel.y + i * chip8_pixel.height,
                chip8_pixel.width,
                chip8_pixel.height,
                (vm->display[i][j]) ? GREEN : BLACK
            );

    DrawRectangleLinesEx(
        (Rectangle){chip8_pixel.x - external_display_line.x,
        chip8_pixel.y - external_display_line.y,
        chip8_pixel.width * CHIP8_DISPLAY_WIDTH + external_display_line.width,
        chip8_pixel.height * CHIP8_DISPLAY_HEIGHT + external_display_line.height},
        line_thickness,
        GREEN
    );
}

static void chip8_draw_hardware_components_area(
    const Chip8VM* vm
){
    const Rectangle hardware_components_box = {
        946,
        26,
        312,
        364
    };

    const float line_thickness = 2.0;
    const int text_x = 10;
    const int text_y = 20;
    const int text_height = 20;
    const int text_widght = 145;
    const int font_spacing = 2;
    const int title_size = 18;

    DrawTextEx(
            font,
            "Registers | Stack",
            (Vector2){hardware_components_box.x,
            hardware_components_box.y - text_widght},
            title_size,
            font_spacing,
            GREEN
        );

    const int stack_size = sizeof(vm->stack)/sizeof(vm->stack[0]);
    int stack_offset = (vm->sp >= stack_size/2)? stack_size/2 : 0;

    for(int i = 0; i < stack_size/2 ; i ++){
        
        chip8_draw_hardware_component(
            (Vector2){hardware_components_box.x + text_x,
            hardware_components_box.y + i * text_height + text_y},
            TextFormat("V%X", i),
            TextFormat("0x%.2X", vm->V[i]),
            GREEN
        );

        switch (i)
        {
        case 0:
            chip8_draw_hardware_component(
                (Vector2){hardware_components_box.x + text_widght + text_x,
                hardware_components_box.y + i * text_height + text_y},
                "I",
                TextFormat("0x%.4X", vm->I),
                GREEN
            );
            break;
        case 1:
            chip8_draw_hardware_component(
                (Vector2){hardware_components_box.x + text_widght + text_x,
                hardware_components_box.y + i * text_height + text_y},
                "PC",
                TextFormat("0x%.4X", vm->pc),
                GREEN
            );
            break;
        case 2:
            chip8_draw_hardware_component(
                (Vector2){hardware_components_box.x + text_widght + text_x,
                hardware_components_box.y + i * text_height + text_y},
                "SP",
                TextFormat("0x%.2X", vm->sp),
                GREEN
            );
            break;
        case 4:
            chip8_draw_hardware_component(
                (Vector2){hardware_components_box.x + text_widght + text_x,
                hardware_components_box.y + i * text_height + text_y},
                "DT",
                TextFormat("0x%.2X", vm->delay_timer),
                GREEN
            );
            break;
        case 5:
            chip8_draw_hardware_component(
                (Vector2){hardware_components_box.x + text_widght + text_x,
                hardware_components_box.y + i * text_height + text_y},
                "ST",
                TextFormat("0x%.2X", vm->sound_timer),
                GREEN
            );
            break;
        default:
            break;
        }

        
        if (i >= 8){
            chip8_draw_hardware_component(
                (Vector2){hardware_components_box.x + text_widght + text_x,
                hardware_components_box.y + i * text_height + text_y},
                TextFormat("S%.1X", stack_offset),
                TextFormat("0x%.4X", vm->stack[stack_offset]),
                GREEN
            );
            stack_offset += 1;
        }
        
    }

    DrawRectangleLinesEx(
        hardware_components_box,
        line_thickness,
        GREEN
    );
}

static void chip8_draw_memory_line(
    const Vector2 pos,
    const char* instruction, 
    const char* memory_address, 
    const Color color
){
    const int mem_addr_spacing = 75;
    const int instruction_spacing = 108;
    const int font_size = 12;
    const int font_spacing = 2;

    DrawTextEx(
        font,
        memory_address,
        pos,
        font_size,
        font_spacing,
        color
    );
    DrawTextEx(
        font,
        "-",
        (Vector2){
            pos.x + mem_addr_spacing,
            pos.y
        },
        font_size,
        font_spacing,
        color
    );
    DrawTextEx(
        font,
        instruction,
        (Vector2){
            pos.x + instruction_spacing,
            pos.y
        },
        font_size,
        font_spacing,
        color
    );
}

static void chip8_draw_memory_area(
    const Chip8VM* vm
){
    const Rectangle memory_box = {
        946,
        420,
        312,
        280
    };

    const float line_thickness = 2.0;
    const int title_size = 18;
    const int font_spacing = 2;
    const int displayed_lines = 14;
    const int text_height = 19;
    const int text_x = 10;
    const int text_y = 12;


    DrawTextEx(
        font,
        "Memory",
        (Vector2){memory_box.x,
        memory_box.y - 20},
        title_size,
        font_spacing,
        GREEN
    );
    
    int mem_offset = (vm->pc - PC_INITIAL_OFFSET)/2;
    
    if (mem_offset > vm->memory.rom_size/2 - displayed_lines)
        mem_offset = vm->memory.rom_size/2 - displayed_lines;
    

    for (int text_offset = 0; 
        text_offset < displayed_lines; 
        mem_offset ++, text_offset++
    ){
        chip8_draw_memory_line(
            (Vector2){
                memory_box.x + text_x,
                memory_box.y + text_y + text_offset * text_height
            },
            vm->dissasembly[mem_offset],
            TextFormat(
                "0x%.4X", (mem_offset + PC_INITIAL_OFFSET) * 2
            ),
            GREEN
        );
    }

    DrawRectangleLinesEx(
        memory_box,
        line_thickness,
        GREEN
    );
            
}

#define X(key_name, key_value)      \
    if (IsKeyDown(key_name)){       \
        vm->keys[key_value] = 1;    \
        return 1;                   \
    }

int chip8_is_key_down(
    Chip8VM* vm
){
    CHIP8_KEYS_TABLE(X)
    return 0;
}

#undef X


void chip8_update_key_map(
    Chip8VM* vm
){
    #define X(key_name, key_value)      \
        if( IsKeyDown(key_name) )       \
            vm->keys[key_value] = 1;    

    CHIP8_KEYS_TABLE(X)

    #undef X 

    #define X(key_name, key_value)      \
        if( IsKeyUp(key_name) )         \
            vm->keys[key_value] = 0;
    
    CHIP8_KEYS_TABLE(X)

    #undef X

}

void chip8_update_display(
    Chip8VM* vm
){
    assert(vm != NULL);

    BeginDrawing();

        ClearBackground(BLACK);

        chip8_draw_display_area(vm);
        
        chip8_draw_hardware_components_area(vm);

        chip8_draw_memory_area(vm);
        
        
    EndDrawing();
}

void chip8_construct_display(){
    SetTraceLogLevel(LOG_ERROR); 
    InitWindow(DISPLAY_WIDHT, DISPLAY_HEIGHT, "CHIP-8 emulator");
    font = LoadFont("assets/fonts/liner BMP14.TTF");
    SetTargetFPS(60);
}

void chip8_destroy_display(){
    CloseWindow();
}
