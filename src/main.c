#include"things.h"
extern Gui gui;
extern Pen pen;
extern enum State state;
extern enum Window window;
extern Sym sym;
int main(){
    //Initialize Window
    SetConfigFlags(FLAG_MSAA_4X_HINT);
    InitWindow(gui.screenwidth,gui.screenheight,"paint");
    //-------------------------------------------------------------------------------------------//
    //initialize variables need by different tools
    //ratio chain
    bool ratiolock = false;

    //reference
    Texture2D reference = LoadTexture("textures");
    bool reference_on = false;
    Rectangle reference_destrect;
    Vector2 reference_origin;
    float reference_rotation = 0;
    float rotation_offset = 0;
    Color reference_tint = (Color){255,255,255,100};
    bool rotate_reference = 0;
    Vector2 moveref_p;

    //switch case
    Vector2 pos1,pos2;
    int stateflag = 0,isreleased = 0;

    //basics
    Vector2 pos = {0,0};  
    bool curdown = false;
    int moveslider = 0;
    int uptextbox = 0;

    Button_LoadTextures();
    
    //drawing
    vector qsplines = GiveVector();
    vector lines = GiveVector();
    vector points = GiveVector(); 
    
    //eraser
    bool eraser_on = false;

    //sym
    bool move_sym_pos1 = false;
    bool move_sym_pos2 = false;

    //screen
    pixel* screen = (pixel*)malloc(gui.screenwidth*gui.screenheight*sizeof(pixel));
    pixel* mask = (pixel*)malloc(gui.screenwidth*gui.screenheight*sizeof(pixel));
    ClearMask(mask);
    ClearScreen(screen,&qsplines,&lines,&points);
    CanvasFitDim(screen,550,550);
    float deltatime = 0.016;

    SetTargetFPS(60);
    //---------------------------------------------------------------------------------------------------------//
    while(!WindowShouldClose()){  
        
        switch (window){             // controls the current window we are on
        //------------------------------------------------------------------------------------------------------------------------------------//
        //switch case for paint begins
        case paint:

        pos = GetMousePosition();
        switch (state){  // switch case tracking active tools in paint application
        //-------------------------------------------------------------------------------------------------------------------------------------//
        //bottons have different tint when they are pressed so the cases for the tools also change that
        //-------------------------------------------------------------------------------------------------------------------------//
        case moveref:
        if(IsMouseButtonPressed(MOUSE_BUTTON_LEFT)){
            if(CheckCollisionPointRec(pos,(Rectangle){gui.buttons[15].top_left.x,gui.buttons[15].top_left.y,gui.buttons[15].dim.x,gui.buttons[15].dim.y})){
                state = normal;
                gui.buttons[15].ispressed = false;
                break;
            }
            else moveref_p = Vector2Minus((Vector2){reference_destrect.x,reference_destrect.y},pos);
        }
        if(IsMouseButtonDown(MOUSE_BUTTON_LEFT)){
            reference_destrect.x = Vector2Sum(pos,moveref_p).x;
            reference_destrect.y = Vector2Sum(pos,moveref_p).y;
        }
        break;
        case eyedropper:
        if(IsMouseButtonPressed(MOUSE_BUTTON_LEFT)){
            if(CheckCollisionPointRec(pos,(Rectangle){gui.buttons[13].top_left.x,gui.buttons[13].top_left.y,gui.buttons[13].dim.x,gui.buttons[13].dim.y})){
                state = normal;
                gui.buttons[13].ispressed = false;
            }
            if(InCanvas(pos)){
                gui.buttons[13].ispressed = false;
                pen.color = screen[(int)pos.x + gui.screenwidth*(int)pos.y].color;
                gui.sliders[0].slide = pen.color.r/255.0;
                gui.sliders[1].slide = pen.color.g/255.0;
                gui.sliders[2].slide = pen.color.b/255.0;
                state = normal;
            }
        }
        break;
        case Bucket:
        if(IsMouseButtonPressed(MOUSE_BUTTON_LEFT)){
            if(CheckCollisionPointRec(pos,(Rectangle){gui.buttons[11].top_left.x,gui.buttons[11].top_left.y,gui.buttons[11].dim.x,gui.buttons[11].dim.y})){
                state = normal;
                gui.buttons[11].ispressed = false;
            }
            if(InCanvas(pos)){
                gui.buttons[11].ispressed = false;
                FillBucket(screen,pos);
                state = normal;
            }
        }
        break;
        //---------------------------------------------------------------------------------------------------------------------//
        /*  TWO POINT TOOLS
            the tools rect,hollowrect,ellipse,hollowellipse,line are of a type I like to call two point tools
            
            These all take two mouseclicks and record their positions in pos1,pos2
            then call a function accordingly with pos1 and pos2 as part of the arguments
            MOST OF THESE TOOLS ALSO CALL DIFFERENT FUNCTIONS DEPENDING ON IF SYMETTRY IS ON*/
        //--------------------------------------------------------------------------------------------------------------------//
        case HollowEllipse:
        if(IsMouseButtonReleased(MOUSE_BUTTON_LEFT)){isreleased = 1;}
        if(CheckCollisionPointRec(pos,(Rectangle){gui.buttons[12].top_left.x,gui.buttons[12].top_left.y,gui.buttons[12].dim.x,gui.buttons[12].dim.y})&& IsMouseButtonPressed(MOUSE_BUTTON_LEFT) && isreleased){
            state = normal;
            stateflag = 0;
            gui.buttons[12].ispressed = false;
        }
        if(stateflag == 1
        && pos.x-1 < gui.CanvasBottomRight.x
        && pos.x-1 > gui.CanvasTopLeft.x                   //get pos2
        && pos.y-1 > gui.CanvasTopLeft.y
        && pos.y < gui.CanvasBottomRight.y
        && IsMouseButtonPressed(MOUSE_BUTTON_LEFT)
        && isreleased){
            pos2 = pos;
            stateflag = 2;
        }
        if(stateflag == 0
        && pos.x-1 < gui.CanvasBottomRight.x
        && pos.x-1 > gui.CanvasTopLeft.x                  //get pos1
        && pos.y-1 > gui.CanvasTopLeft.y
        && pos.y < gui.CanvasBottomRight.y
        && IsMouseButtonPressed(MOUSE_BUTTON_LEFT)
        && isreleased){
            pos1 = pos;
            stateflag = 1;
        }
   
        if(stateflag == 2){
            gui.buttons[12].ispressed = false;
            if(IsSymOn()){
                DrawSymHollowEllipse(screen,pos1,pos2,pen.thickness,pen.color);
            }
            else{
                DrawHollowEllipse(screen,pos1,pos2,pen.thickness,pen.color);
            }
            state = normal;
            stateflag = 0;
            isreleased = 0;
        }
        break;

        case HollowRect:
        if(IsMouseButtonReleased(MOUSE_BUTTON_LEFT)){isreleased = 1;}
        if(CheckCollisionPointRec(pos,(Rectangle){gui.buttons[10].top_left.x,gui.buttons[10].top_left.y,gui.buttons[10].dim.x,gui.buttons[10].dim.y})&& IsMouseButtonPressed(MOUSE_BUTTON_LEFT) && isreleased){
            state = normal;
            stateflag = 0;
            gui.buttons[10].ispressed = false;
        }
        if(stateflag == 1
        && pos.x-1 < gui.CanvasBottomRight.x
        && pos.x-1 > gui.CanvasTopLeft.x
        && pos.y-1 > gui.CanvasTopLeft.y
        && pos.y < gui.CanvasBottomRight.y
        && IsMouseButtonPressed(MOUSE_BUTTON_LEFT)
        && isreleased){
            pos2 = pos;
            stateflag = 2;
        }
        if(stateflag == 0
        && pos.x-1 < gui.CanvasBottomRight.x
        && pos.x-1 > gui.CanvasTopLeft.x
        && pos.y-1 > gui.CanvasTopLeft.y
        && pos.y < gui.CanvasBottomRight.y
        && IsMouseButtonPressed(MOUSE_BUTTON_LEFT)
        && isreleased){
            pos1 = pos;
            stateflag = 1;
        }
        if(stateflag == 2){
            gui.buttons[10].ispressed = false;
            if(IsSymOn){
                DrawSymHollowRectangle(screen,pos1,pos2,pen.thickness,pen.color);
            }
            else{
                DrawHollowRectangle(screen,pos1,pos2,pen.thickness,pen.color);
            }
            state = normal;
            stateflag = 0;
            isreleased = 0;
        }
        break;
        case rect:
        if(IsMouseButtonReleased(MOUSE_BUTTON_LEFT)){isreleased = 1;}
        if(CheckCollisionPointRec(pos,(Rectangle){gui.buttons[3].top_left.x,gui.buttons[3].top_left.y,gui.buttons[3].dim.x,gui.buttons[3].dim.y})&& IsMouseButtonPressed(MOUSE_BUTTON_LEFT) && isreleased){
            state = normal;
            stateflag = 0;  
            gui.buttons[3].ispressed = false;
        }
        if(stateflag == 1
        && pos.x-1 < gui.CanvasBottomRight.x
        && pos.x-1 > gui.CanvasTopLeft.x                              //get pos2
        && pos.y-1 > gui.CanvasTopLeft.y
        && pos.y < gui.CanvasBottomRight.y
        && IsMouseButtonPressed(MOUSE_BUTTON_LEFT)
        && isreleased){
            pos2 = pos;
            stateflag = 2;
        }
        if(stateflag == 0
        && pos.x-1 < gui.CanvasBottomRight.x
        && pos.x-1 > gui.CanvasTopLeft.x                                //get pos1
        && pos.y-1 > gui.CanvasTopLeft.y
        && pos.y < gui.CanvasBottomRight.y
        && IsMouseButtonPressed(MOUSE_BUTTON_LEFT)
        && isreleased){
            pos1 = pos;
            stateflag = 1;
        }
        if(stateflag == 2){
            if(IsSymOn()){
                MyDrawSymRectangle(screen,pos1,pos2,pen.color);
            }
            else{
                MyDrawRectangle(screen,pos1,pos2,pen.color);
            }
            state = normal;
            stateflag = 0;
            isreleased = 0;
            gui.buttons[3].ispressed = false;
        }
        break;
        case ellipse:
        if(IsMouseButtonReleased(MOUSE_BUTTON_LEFT)){isreleased = 1;}
        if(CheckCollisionPointRec(pos,(Rectangle){gui.buttons[5].top_left.x,gui.buttons[5].top_left.y,gui.buttons[5].dim.x,gui.buttons[5].dim.y}) && IsMouseButtonPressed(MOUSE_BUTTON_LEFT) && isreleased){
            state = normal;
            stateflag = 0; 
            gui.buttons[5].ispressed = false;   
        }
        if(stateflag == 1
        && pos.x-1 < gui.CanvasBottomRight.x
        && pos.x-1 > gui.CanvasTopLeft.x
        && pos.y-1 > gui.CanvasTopLeft.y
        && pos.y < gui.CanvasBottomRight.y
        && IsMouseButtonPressed(MOUSE_BUTTON_LEFT) 
        && isreleased){
            pos2 = pos;
            stateflag = 2;
        }
        if(stateflag == 0
        && pos.x-1 < gui.CanvasBottomRight.x
        && pos.x-1 > gui.CanvasTopLeft.x
        && pos.y-1 > gui.CanvasTopLeft.y
        && pos.y < gui.CanvasBottomRight.y
        && IsMouseButtonPressed(MOUSE_BUTTON_LEFT)
        && isreleased){
            pos1 = pos;
            stateflag = 1;
        }
        if(stateflag == 2){
            if(IsSymOn()){
                MyDrawSymEllipse(screen,pos1,pos2,pen.color);
            }
            else{
                MyDrawEllipse(screen,pos1,pos2,pen.color);
            }
            state = normal;
            stateflag = 0;
            isreleased = 0;

            gui.buttons[5].ispressed = false;
        }
        break;
        case line:
        if(IsMouseButtonReleased(MOUSE_BUTTON_LEFT)){isreleased = 1;}
        if(CheckCollisionPointRec(pos,(Rectangle){gui.buttons[6].top_left.x,gui.buttons[6].top_left.y,gui.buttons[6].dim.x,gui.buttons[6].dim.y})&& IsMouseButtonPressed(MOUSE_BUTTON_LEFT) && isreleased){
            state = normal;
            stateflag = 0;
            gui.buttons[6].ispressed = false;
        }
        if(stateflag == 1
        && pos.x-1 < gui.CanvasBottomRight.x
        && pos.x-1 > gui.CanvasTopLeft.x
        && pos.y-1 > gui.CanvasTopLeft.y
        && pos.y < gui.CanvasBottomRight.y
        && IsMouseButtonPressed(MOUSE_BUTTON_LEFT)
        && isreleased){
            pos2 = pos;
            stateflag = 2;
        }
        if(stateflag == 0
        && pos.x-1 < gui.CanvasBottomRight.x
        && pos.x-1 > gui.CanvasTopLeft.x
        && pos.y-1 > gui.CanvasTopLeft.y
        && pos.y < gui.CanvasBottomRight.y
        && IsMouseButtonPressed(MOUSE_BUTTON_LEFT)
        && isreleased){
            pos1 = pos;
            stateflag = 1;
        }
        if(stateflag == 2){
            Vector_Add50e(&lines,pos1);
            Vector_Add50e(&lines,pos2);
            if(IsSymOn()){
                Vector_Add50e(&lines,GetSymPoint(pos1));
                Vector_Add50e(&lines,GetSymPoint(pos2));
            }
            state = normal;
            stateflag = 0;
            isreleased = 0;
            gui.buttons[6].ispressed = false;
        }
        break;
        //-----------------------------------------------------------------------------------------------------------------------------------//
        //DEFAULT CASE FOR PAINT
        //-----------------------------------------------------------------------------------------------------------------------------------//
        case normal:
        // check if hovering over text boxes
        for(int i = 0;i<gui.num_textboxes;i++){
            textbox t = gui.textboxes[i];
            if(CheckCollisionPointRec(pos,(Rectangle){t.top_left.x,t.top_left.y,t.dim.x,t.dim.y})){
                uptextbox = i + 1;
            }
        }
        
        //on click
        if(IsMouseButtonPressed(MOUSE_BUTTON_LEFT)){
            curdown = true;
            //check if sym line is bieng moved
            if(CheckCollisionPointCircle(pos,sym.pos1,sym.r1)) move_sym_pos1 = true;
            if(CheckCollisionPointCircle(pos,sym.pos2,sym.r1)) move_sym_pos2 = true;

            //rotate reference
            if(CheckCollisionPointCircle(pos,(Vector2){reference_destrect.x,reference_destrect.y},Vector2Len(reference_origin)+15) && !CheckCollisionPointCircle(pos,(Vector2){reference_destrect.x,reference_destrect.y},Vector2Len(reference_origin)) && reference_on){
                rotate_reference = true;
                reference_rotation -= rotation_offset;
                if(pos.y >= reference_destrect.y){
                    rotation_offset = PI - acosf(Vector2Dot((Vector2){100,0},Vector2Minus(pos,(Vector2){reference_destrect.x,reference_destrect.y}))/(100*Vector2Len(Vector2Minus(pos,(Vector2){reference_destrect.x,reference_destrect.y}))));
                }
                else{
                    rotation_offset = PI + acosf(Vector2Dot((Vector2){100,0},Vector2Minus(pos,(Vector2){reference_destrect.x,reference_destrect.y}))/(100*Vector2Len(Vector2Minus(pos,(Vector2){reference_destrect.x,reference_destrect.y}))));
                }
                rotation_offset = (rotation_offset*360) / (2*PI);
                printf("%f\n",rotation_offset);
                reference_rotation += rotation_offset;
                if(reference_rotation >360) reference_rotation -= 360;
            }

            //buttons
            /*  The Button_IfpressedChecks if a button is pressed and if it is calls the function associated with it using arguments provided bellow
                */
            Button_Ifpressed(gui.buttons[0],pos,screen,&qsplines,&lines,&points);
            Button_Ifpressed(gui.buttons[1],pos,screen,gui.textboxes[1].text,NULL,NULL);
            Button_Ifpressed(gui.buttons[2],pos,screen,gui.textboxes[2].text,NULL,NULL);
            Button_Ifpressed(gui.buttons[3],pos,screen,&curdown,NULL,gui.buttons + 3);
            Button_Ifpressed(gui.buttons[4],pos,screen,gui.textboxes[3].text,gui.textboxes[4].text,NULL);
            Button_Ifpressed(gui.buttons[5],pos,&curdown,NULL,NULL,gui.buttons + 5);
            Button_Ifpressed(gui.buttons[6],pos,&curdown,NULL,NULL,gui.buttons + 6);
            Button_Ifpressed(gui.buttons[7],pos,&ratiolock,NULL,NULL,gui.buttons + 7);
            Button_Ifpressed(gui.buttons[8],pos,&window,&curdown,NULL,NULL);
            Button_Ifpressed(gui.buttons[9],pos,&eraser_on,NULL,NULL,gui.buttons + 9);
            Button_Ifpressed(gui.buttons[10],pos,&curdown,NULL,NULL,gui.buttons + 10);
            Button_Ifpressed(gui.buttons[11],pos,&curdown,NULL,NULL,gui.buttons + 11);
            Button_Ifpressed(gui.buttons[12],pos,&curdown,NULL,NULL,gui.buttons + 12);
            Button_Ifpressed(gui.buttons[13],pos,&curdown,NULL,NULL,gui.buttons + 13);
            Button_Ifpressed(gui.buttons[14],pos,&reference,&reference_on,&reference_destrect,&reference_origin);
            Button_Ifpressed(gui.buttons[15],pos,&curdown,&moveref_p,&reference_destrect,gui.buttons + 15);
            Button_Ifpressed(gui.buttons[16],pos,&curdown,NULL,NULL,NULL);
            Button_Ifpressed(gui.buttons[17],pos,&curdown,NULL,NULL,NULL);
            Button_Ifpressed(gui.buttons[18],pos,&curdown,NULL,NULL,NULL);
            
            //sliders
            for(int i = 0;i<gui.num_sliders;i++){
                slider s = gui.sliders[i];
                if(CheckCollisionPointRec(pos,(Rectangle){s.pos.x + s.slide * s.bar_dim.x - s.rect_dim.x/2,s.pos.y - (s.rect_dim.y - s.bar_dim.y)/2,s.rect_dim.x,s.rect_dim.y})){
                    moveslider = i+1;
                }
            }
        }
        if(IsMouseButtonReleased(MOUSE_BUTTON_LEFT)){
            curdown = false;
            rotate_reference = false;
        }

        if(curdown){
            //set the color to sliders if eraser is not on
            if(!eraser_on){
                pen.color = (Color){(int)(255 * gui.sliders[0].slide),
                                    (int)(255 * gui.sliders[1].slide),
                                    (int)(255 * gui.sliders[2].slide),255};
            }
            
            if(rotate_reference){
                if(pos.y >= reference_destrect.y){
                    rotation_offset = PI - acosf(Vector2Dot((Vector2){100,0},Vector2Minus(pos,(Vector2){reference_destrect.x,reference_destrect.y}))/(100*Vector2Len(Vector2Minus(pos,(Vector2){reference_destrect.x,reference_destrect.y}))));
                }
                else{
                    rotation_offset = PI + acosf(Vector2Dot((Vector2){100,0},Vector2Minus(pos,(Vector2){reference_destrect.x,reference_destrect.y}))/(100*Vector2Len(Vector2Minus(pos,(Vector2){reference_destrect.x,reference_destrect.y}))));
                }
                rotation_offset = (rotation_offset*360) / (2*PI);
            }
            
            if(moveslider){
                slider s = gui.sliders[moveslider-1];
                if(CheckCollisionPointRec(pos,(Rectangle){s.pos.x,s.pos.y - (s.rect_dim.y - s.bar_dim.y)/2,s.bar_dim.x,s.rect_dim.y})){
                    gui.sliders[moveslider-1].slide = (pos.x-s.pos.x)/s.bar_dim.x;
                }
                else moveslider = 0;
            }

            // record mouse position if mouse is drawing on the canvas
            if(pos.x < gui.CanvasBottomRight.x && pos.x > gui.CanvasTopLeft.x && pos.y > gui.CanvasTopLeft.y && pos.y < gui.CanvasBottomRight.y && !move_sym_pos1 && !move_sym_pos2 && !rotate_reference){
                screen[(int)(pos.y * gui.screenwidth + pos.x)].color = (Color){0,0,0,255};
                Vector_Add50e(&points,(Vector2){pos.x,pos.y});
            }
            //--------------------------------------------------------------------------------------------------------------------------------------------------------------//
            /*  TO DRAW we take the points recorded and draw quadric beziers between three points and if only two points are left by the end we draw a line between the */
            //----------------------------------------------------------------------------------------------------------------------------------------------------------------//
            if(points.len==3){
                Vector_Add50e(&qsplines,(points.arr)[0]);                    
                Vector_Add50e(&qsplines,(points.arr)[1]);                    
                Vector_Add50e(&qsplines,(points.arr)[2]);
                points.arr[0] = points.arr[2];
                Vector_pop(&points);
                Vector_pop(&points);
            }
        }
        if(!curdown){
            if(points.len != 0){
                int i  = 0;
                for(;points.len-i>=3;i+=2){               
                    Vector_Add50e(&qsplines,(points.arr)[i]);                    
                    Vector_Add50e(&qsplines,(points.arr)[i+1]);                    
                    Vector_Add50e(&qsplines,(points.arr)[i+2]);                    
                }
                if(points.len - i == 2){
                    Vector_Add50e(&lines,(points.arr)[i]);
                    Vector_Add50e(&lines,(points.arr)[i+1]);
                }
                Vector_Empty(&points);
            }
        }

        if(move_sym_pos1){
            sym.pos1 = pos;
            if(IsMouseButtonReleased(MOUSE_BUTTON_LEFT)) move_sym_pos1 = false;
        }
        if(move_sym_pos2){
            sym.pos2 = pos;
            if(IsMouseButtonReleased(MOUSE_BUTTON_LEFT)) move_sym_pos2 = false;
        }

        if(uptextbox){
            if(uptextbox == 1){
                int key = GetKeyPressed();
                    if(key == KEY_BACKSPACE && gui.textboxes[0].len != 0 ){
                        gui.textboxes[0].text[gui.textboxes[0].len-1] = '\0';
                        gui.textboxes[0].len = gui.textboxes[0].len - 1;
                    }
                    if(key>= 48 && key<=57 && gui.textboxes[0].len != 3){
                        gui.textboxes[0].text[gui.textboxes[0].len] = (char)key;
                        gui.textboxes[0].len = gui.textboxes[0].len + 1;
                    }
                    if(atoi(gui.textboxes[0].text)>150){
                        gui.textboxes[0].text[0] = '1';
                        gui.textboxes[0].text[1] = '5';
                        gui.textboxes[0].text[2] = '0';
                    }
                    pen.thickness = atoi(gui.textboxes[0].text);
            }
            if(uptextbox == 2){
                int key = GetKeyPressed();
                    if(key == KEY_BACKSPACE && gui.textboxes[1].len != 0 ){
                        gui.textboxes[1].text[gui.textboxes[1].len-1] = '\0';
                        gui.textboxes[1].len = gui.textboxes[1].len - 1;
                    }
                    if(key>= 32 && key<=126 && gui.textboxes[1].len != 40){
                        gui.textboxes[1].text[gui.textboxes[1].len] = (char)key;
                        gui.textboxes[1].len = gui.textboxes[1].len + 1;
                    }
            }
            if(uptextbox == 3){
                int key = GetKeyPressed();
                    if(key == KEY_BACKSPACE && gui.textboxes[2].len != 0 ){
                        gui.textboxes[2].text[gui.textboxes[2].len-1] = '\0';
                        gui.textboxes[2].len = gui.textboxes[2].len - 1;
                    }
                    if(key>= 32 && key<=126 && gui.textboxes[2].len != 40){
                        gui.textboxes[2].text[gui.textboxes[2].len] = (char)key;
                        gui.textboxes[2].len = gui.textboxes[2].len + 1;
                    }
            }
            if(uptextbox == 4){
                int key = GetKeyPressed();
                    if(key == KEY_BACKSPACE && gui.textboxes[3].len != 0 ){
                        gui.textboxes[3].text[gui.textboxes[3].len-1] = '\0';
                        gui.textboxes[3].len = gui.textboxes[3].len - 1;
                    }
                    if(key>= 48 && key<=57 && gui.textboxes[3].len != 3){
                        gui.textboxes[3].text[gui.textboxes[3].len] = (char)key;
                        gui.textboxes[3].len = gui.textboxes[3].len + 1;
                    }
                    if(atoi(gui.textboxes[3].text)>900){
                        sprintf(gui.textboxes[3].text,"900");
                    }
                    if(ratiolock){
                        if(atoi(gui.textboxes[3].text)*(9.0/16.0) > 650){
                            sprintf(gui.textboxes[4].text,"650");
                        }
                        else{
                            sprintf(gui.textboxes[4].text,"%d",(int)(atoi(gui.textboxes[3].text)*(9.0/16.0)));
                        }
                        gui.textboxes[4].len = strlen(gui.textboxes[4].text);
                    }
            }
            if(uptextbox == 5){
                int key = GetKeyPressed();
                    if(key == KEY_BACKSPACE && gui.textboxes[4].len != 0 ){
                        gui.textboxes[4].text[gui.textboxes[4].len-1] = '\0';
                        gui.textboxes[4].len = gui.textboxes[4].len - 1;
                    }
                    if(key>= 48 && key<=57 && gui.textboxes[4].len != 3){
                        gui.textboxes[4].text[gui.textboxes[4].len] = (char)key;
                        gui.textboxes[4].len = gui.textboxes[4].len + 1;
                    }
                    if(atoi(gui.textboxes[4].text)>650){
                        sprintf(gui.textboxes[4].text,"650");
                    }
                    if(ratiolock){
                        if(atoi(gui.textboxes[4].text)*(16.0/9.0) > 900){
                            sprintf(gui.textboxes[3].text,"900");
                        }
                        else{
                            sprintf(gui.textboxes[3].text,"%d",(int)(atoi(gui.textboxes[4].text)*(16.0/9.0)));
                        }
                        gui.textboxes[3].len = strlen(gui.textboxes[3].text);
                    }
            }
            if(uptextbox == 6){
                int key = GetKeyPressed();
                    if(key == KEY_BACKSPACE && gui.textboxes[5].len != 0 ){
                        gui.textboxes[5].text[gui.textboxes[5].len-1] = '\0';
                        gui.textboxes[5].len = gui.textboxes[5].len - 1;
                    }
                    if(key>= 32 && key<=126 && gui.textboxes[5].len != 40){
                        gui.textboxes[5].text[gui.textboxes[5].len] = (char)key;
                        gui.textboxes[5].len = gui.textboxes[5].len + 1;
                    }
            }
            uptextbox = 0;
        }
      
        AddLines(screen,&lines);
        AddQSplines(screen,&qsplines);
        break;
 
        }
        BeginDrawing();
            DrawScreen(screen);
            DrawLabels();
            DrawSymLine(pos);
            if(reference_on){
                DrawRing((Vector2){reference_destrect.x,reference_destrect.y},
                        Vector2Len(reference_origin),Vector2Len(reference_origin) + 15
                        ,0,360,40,(Color){0,0,0,100});
                DrawTexturePro(reference,
                              (Rectangle){0,0,reference.width,reference.height},
                              reference_destrect,
                              reference_origin,
                              reference_rotation - rotation_offset,
                              reference_tint);
            }
        EndDrawing();
        }
    }
    void Button_UnloadTextures();
    CloseWindow();
    return 0;
}