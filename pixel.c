#include"things.h"
extern Gui gui;
extern Pen pen;

void MyDrawPixelCanvas(pixel* screen,Vector2 pos,Color color){
    if(pos.x < gui.CanvasBottomRight.x && pos.x > gui.CanvasTopLeft.x && pos.y > gui.CanvasTopLeft.y && pos.y < gui.CanvasBottomRight.y){
        screen[(int)(gui.screenwidth)*(int)pos.y + (int)(pos.x)] = (pixel){color};
    }
}

void MyDrawRectangle(pixel* screen,Vector2 top_left,Vector2 bottom_right,Color color){
    for(int i = 0;i<=bottom_right.x-top_left.x;i++){
        for(int j = 0;j<=bottom_right.y-top_left.y;j++){
            screen[(int)(top_left.x + top_left.y*gui.screenwidth + i + j*gui.screenwidth)].color = color;
        }
    }
}

void MyDrawCircle(pixel* screen,Vector2 center,int radius,Color color){
    for(int i = 0;i<radius;i++){
        int k = radius*radius;
        for(int j = 0;i*i + j*j <= k;j++){
            MyDrawPixelCanvas(screen,Vector2Sum(center,(Vector2){j,i}),color);
            MyDrawPixelCanvas(screen,Vector2Sum(center,(Vector2){-j,-i}),color);
            MyDrawPixelCanvas(screen,Vector2Sum(center,(Vector2){j,-i}),color);
            MyDrawPixelCanvas(screen,Vector2Sum(center,(Vector2){-j,i}),color);
        }
    }
}

void ClearScreen(pixel* screen,vector* qsplines,vector* lines,vector* points){
    for(int i = 0;i<gui.screenwidth*gui.screenheight;i++){
        screen[i] = (pixel){WHITE};
    }
    Vector_Empty(qsplines);
    Vector_Empty(lines);
    Vector_Empty(points);
    
}

void ClearCanvas(pixel* screen,vector* qsplines,vector* lines,vector* points){
    for(int i = gui.CanvasTopLeft.y+1;i < gui.CanvasBottomRight.y;i++){
        for(int j = gui.CanvasTopLeft.x+1; j < gui.CanvasBottomRight.x;j++){
            screen[i*gui.screenwidth + j] = (pixel){WHITE};
        }
        
    }
    Vector_Empty(qsplines);
    Vector_Empty(lines);
    Vector_Empty(points);
    
}

void ClearCanvasPixels(pixel* screen){
    for(int i = gui.CanvasTopLeft.y+1;i < gui.CanvasBottomRight.y;i++){
        for(int j = gui.CanvasTopLeft.x+1; j < gui.CanvasBottomRight.x;j++){
            screen[i*gui.screenwidth + j] = (pixel){WHITE};
        }   
    }
}

void CanvasFitDim(pixel* screen,int width,int height){
    AddGuiBase(screen,WHITE);
    ClearCanvasPixels(screen);
    gui.CanvasTopLeft = Vector2Sum(gui.CanvasCenter,(Vector2){-1*width/2 - 1,-1*height/2 -1});
    gui.CanvasBottomRight = Vector2Sum(gui.CanvasTopLeft,(Vector2){width+1,height+1});
    AddGuiBase(screen,RED);
}

void DrawScreen(pixel* screen){
    ClearBackground(RAYWHITE);
    for(int i = gui.CanvasTopLeft.x-gui.CanvasBorderThickness+1;i<=gui.CanvasBottomRight.x+gui.CanvasBorderThickness-1;i++){
        for(int j = gui.CanvasTopLeft.y-gui.CanvasBorderThickness+1;j<=gui.CanvasBottomRight.y+gui.CanvasBorderThickness-1;j++){
            DrawRectangle(i,j,1,1,screen[gui.screenwidth*j + i].color);
        }
        
    }

    // for(int i = 0;i<gui.screenwidth*gui.screenheight;i++){
    //     DrawRectangle(i%gui.screenwidth,i/gui.screenwidth,1,1,(screen[i]).color);
    // }
    for(int i = 0;i<gui.num_buttons;i++){
        DrawTextureV(gui.buttons[i].texture,gui.buttons[i].top_left,WHITE);
    }
    for(int i = 0;i<gui.num_sliders;i++){
        DrawSlider(gui.sliders[i]);
    }
    for(int i = 0;i<gui.num_textboxes;i++){
        Draw_Textboxes(gui.textboxes[i]);
    }
}

void Draw_Textboxes(textbox t){
    DrawRectangle(t.top_left.x,t.top_left.y,t.dim.x,t.dim.y,BLUE);
    DrawText(t.text,t.top_left.x,t.top_left.y,t.dim.y,RED);
}

void AddGuiBase(pixel* screen,Color color){
    MyDrawRectangle(screen,(Vector2){gui.CanvasTopLeft.x-gui.CanvasBorderThickness,gui.CanvasTopLeft.y-gui.CanvasBorderThickness},(Vector2){gui.CanvasBottomRight.x+gui.CanvasBorderThickness,gui.CanvasTopLeft.y},color);
    MyDrawRectangle(screen,(Vector2){gui.CanvasTopLeft.x-gui.CanvasBorderThickness,gui.CanvasBottomRight.y},(Vector2){gui.CanvasBottomRight.x+gui.CanvasBorderThickness,gui.CanvasBottomRight.y+gui.CanvasBorderThickness},color);
    MyDrawRectangle(screen,(Vector2){gui.CanvasTopLeft.x-gui.CanvasBorderThickness,gui.CanvasTopLeft.y},(Vector2){gui.CanvasTopLeft.x,gui.CanvasBottomRight.y},color);
    MyDrawRectangle(screen,(Vector2){gui.CanvasBottomRight.x,gui.CanvasTopLeft.y},(Vector2){gui.CanvasBottomRight.x+gui.CanvasBorderThickness,gui.CanvasBottomRight.y},color); 
}

void DrawQSplines(vector* qsplines){
    for(int i = 0;i<(qsplines->len);i+=3){
        DrawSplineSegmentBezierQuadratic((qsplines->arr)[i],(qsplines->arr)[i+1],(qsplines->arr)[i+2],1,BLACK);
    }
}

void DrawLines(vector* lines){
    for(int i = 0;i<lines->len;i+=2){
        DrawLineV((lines->arr)[i],(lines->arr)[i+1],BLACK);
    }
}

void AddQSplines(pixel* screen,vector* qsplines){
    for(int i = 0;i<(qsplines->len);i+=3){
        for(int k = 0;k<100;k++){
            Vector2 p = GetSplinePointBezierQuad((qsplines->arr)[i],(qsplines->arr)[i+1],(qsplines->arr)[i+2],(float)k/100.0); //k is getting promoted
            //Vector2 p = GetSplinePointBezierQuad((Vector2){200,200},(Vector2){200,300},(Vector2){400,400},(float)k/100);
            MyDrawCircle(screen,p,pen.thickness,pen.color);
        }
    }
    Vector_Empty(qsplines);
}

void AddLines(pixel* screen,vector* lines){
    for(int i = 0;i<lines->len;i+=2){
        for(int k = 0;k<100;k++){
            Vector2 p = GetSplinePointLinear((lines->arr)[i],(lines->arr)[i+1],k/100.0); //k is getting type promoted
            MyDrawCircle(screen,p,pen.thickness,pen.color);
        }
        Vector_Empty(lines);
    }
}













