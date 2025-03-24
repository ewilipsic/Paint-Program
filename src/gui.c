#include"things.h"

char t1[40] = {'1','0'};
char t2[40];
char t3[40];
char t4[40];
char t5[40];
char t6[40];  


textbox textboxes[6] = {(textbox){t1,2,(Vector2){20,556},(Vector2){100,25}}, // pen size
                        (textbox){t2,0,(Vector2){20,470},(Vector2){200,20}}, // savename
                        (textbox){t3,0,(Vector2){20,510},(Vector2){200,20}}, // load name
                        (textbox){t4,0,(Vector2){20,658},(Vector2){100,25}},  // new width
                        (textbox){t5,0,(Vector2){140,658},(Vector2){100,25}}, // new height
                        (textbox){t6,0,(Vector2){20,605},(Vector2){250,25}}}; // reference path

slider sliders[3] = {(slider){(Vector2){20,150},0,(Vector2){250,25},(Color){255,0,0,255},(Vector2){15,50}},
                     (slider){(Vector2){20,210},0,(Vector2){250,25},(Color){0,255,0,255},(Vector2){15,50}},
                     (slider){(Vector2){20,270},0,(Vector2){250,25},(Color){0,0,255,255},(Vector2){15,50}}};

button buttons[19] = {(button){(Vector2){20,21},(Vector2){20,20},&ScreenClearButton,"textures\\clearscreenbutton.png"},
                     (button){(Vector2){240,470},(Vector2){20,20},&SaveAsPpm6,"textures\\savebutton.png"},
                     (button){(Vector2){240,510},(Vector2){20,20},&LoadPpm6,"textures\\loadbutton.png"},
                     (button){(Vector2){41,21},(Vector2){20,20},&RectTool,"textures\\clearscreenbutton.png","textures\\p.png",0,1},
                     (button){(Vector2){250,658},(Vector2){70,25},&NewButton,"textures\\new.png"},
                     (button){(Vector2){41,42},(Vector2){20,20},&EllipseTool,"textures\\ellipsetool.png","textures\\p.png",0,1},
                     (button){(Vector2){20,42},(Vector2){20,20},&LineTool,"textures\\linetool.png","textures\\p.png",0,1},
                     (button){(Vector2){330,658},(Vector2){19,25},&RatioLock,"textures\\ratioc.png","textures\\p.png",0,1},
                     (button){(Vector2){1500,20},(Vector2){100,20},&Bored,"textures\\bored.png"},
                     (button){(Vector2){20,63},(Vector2){20,20},&Eraser,"textures\\eraser.png","textures\\p.png",0,1},
                     (button){(Vector2){41,63},(Vector2){20,20},&HollowRectTool,"textures\\hollowrect.png","textures\\p.png",0,1},
                     (button){(Vector2){20,84},(Vector2){20,20},&FillToolActivation,"textures\\bucket.png","textures\\p.png",0,1},
                     (button){(Vector2){41,84},(Vector2){20,20},&HollowEllipseTool,"textures\\hollowellipsetool.png","textures\\p.png",0,1},
                     (button){(Vector2){20,105},(Vector2){20,20},&EyeDropper,"textures\\eyedropper.png","textures\\p.png",0,1},
                     (button){(Vector2){275,605},(Vector2){25,25},&LoadReference,"textures\\reftool.png"},
                     (button){(Vector2){305,605},(Vector2){25,25},&MoveReference,"textures\\moveref.png","textures\\p.png",0,1},
                     (button){(Vector2){120,410},(Vector2){20,20},&ChangeBrushCircle,"textures\\circlebrush.png"},
                     (button){(Vector2){142,410},(Vector2){20,20},&ChangeBrushSquare,"textures\\squarebrush.png"},
                     (button){(Vector2){164,410},(Vector2){20,20},&ChangeBrushSpray,"textures\\spraybrush.png"},
                     };

Gui gui = {1280,720,
            (Vector2){814,359},
            (Vector2){400,100},(Vector2){1200,650},4,
            buttons,19,
            sliders,3,
            textboxes,6};

Pen pen = {10,BLACK,circle};
Sym sym = {.pos1 = (Vector2){200,20},
           .pos2 = (Vector2){400,20},
           .r1 = 7,
           .r2 = 13,
           .thick = 3,
           .color = LIME
};

enum State state = normal;
enum Window window = paint; 
