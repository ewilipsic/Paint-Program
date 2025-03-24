#include"things.h"
extern Gui gui;
extern Pen pen;
extern enum State state;
#define screen(v) screen[(int)(v.y) * gui.screenwidth + (int)(v.x)].color
#define mask(v) mask[(int)(v.y) * gui.screenwidth + (int)(v.x)].color
#define ColorComp(d,c) d.r == c.r && d.g == c.g && d.b == c.b && d.a == c.a

//Utility functions for vector2
float Vector2Len(Vector2 a){
    return sqrtf(a.x*a.x + a.y*a.y);
}

float Vector2Lensq(Vector2 a){
    return (a.x*a.x + a.y*a.y);
}

Vector2 Vector2Sum(Vector2 a,Vector2 b){
    return (Vector2){a.x+b.x,a.y+b.y};
}

Vector2 Vector2Minus(Vector2 a,Vector2 b){
    return (Vector2){a.x-b.x,a.y-b.y};
}

Vector2 Vector2Mult(float a,Vector2 b){
    return (Vector2){a*b.x,a*b.y};
}

float Vector2Dot(Vector2 a,Vector2 b){
    return a.x*b.x + a.y*b.y;
}

void Button_LoadTextures(){
    for(int i = 0;i<gui.num_buttons;i++){
        gui.buttons[i].texture = LoadTexture(gui.buttons[i].texture_path);
    }
    for(int i = 0;i<gui.num_buttons;i++){
        if(gui.buttons[i].holdable){
        gui.buttons[i].ptexture = LoadTexture(gui.buttons[i].ptexture_path);
        }
    }
    
}

void Button_UnloadTextures(){
    for(int i = 0;i<gui.num_buttons;i++){
        UnloadTexture(gui.buttons[i].texture);
    }
    for(int i = 0;i<gui.num_buttons;i++){
        if(gui.buttons[i].holdable){
        UnloadTexture(gui.buttons[i].ptexture);
        }
    }
}

void Button_Ifpressed(button but,Vector2 cur_pos,void* a,void* b,void* c,void* d){
    if(CheckCollisionPointRec(cur_pos,(Rectangle){but.top_left.x,but.top_left.y,but.dim.x,but.dim.y})){
        but.func(a,b,c,d);
    }
}

//functions for individual buttons
void ScreenClearButton(void* a,void* b,void* c,void* d){
    pixel* screen = (pixel*)a;
    vector* qsplines = (vector*)b;
    vector* lines = (vector*)c;
    vector* points = (vector*)d;
    ClearCanvas(screen,qsplines,lines,points);
}

void SaveAsPpm6(void* a,void* b,void* c,void* d){
    pixel* screen = (pixel*)a;
    char* filename = (char*)b;

    FILE *f = fopen(filename,"w");
    fprintf(f,"P6 %d %d 255\n",(int)gui.CanvasBottomRight.x-(int)gui.CanvasTopLeft.x-1,(int)gui.CanvasBottomRight.y-(int)gui.CanvasTopLeft.y -1);
    fclose(f);
    FILE *f1 = fopen(filename,"ab");
    for(int i = gui.CanvasTopLeft.y+1;i < gui.CanvasBottomRight.y;i++){
        for(int j = gui.CanvasTopLeft.x+1; j < gui.CanvasBottomRight.x;j++){
            char pixel[3] ={screen[i*gui.screenwidth + j].color.r,screen[i*gui.screenwidth + j].color.g,screen[i*gui.screenwidth + j].color.b}; 
            fwrite(pixel,1,3,f1);
        }
    }
    fclose(f1);
}

void LoadPpm6(void* a,void* b,void* c,void* d){
    pixel* screen = (pixel*)a;
    char* filename = (char*)b;

    FILE *f = fopen(filename,"r");
    char w[4] = {0},h[4] = {0},trash[4];
    fscanf(f,"%s %s %s %s",trash,w,h,trash);
    CanvasFitDim(screen,atoi(w),atoi(h));
    if(fgetc(f) == '\n'){
    
    }
    for(int i = gui.CanvasTopLeft.y+1;i < gui.CanvasBottomRight.y;i++){
        for(int j = gui.CanvasTopLeft.x+1; j < gui.CanvasBottomRight.x;j++){
            screen[i*gui.screenwidth + j] = (pixel){(Color){fgetc(f),fgetc(f),fgetc(f),255}};
        }
    }
    fclose(f);
}

void DrawSlider(slider s){
    DrawRectangle(s.pos.x + s.slide * s.bar_dim.x - s.rect_dim.x/2,s.pos.y - (s.rect_dim.y - s.bar_dim.y)/2,s.rect_dim.x,s.rect_dim.y,BLACK);
    DrawRectangleGradientH(s.pos.x,s.pos.y,s.bar_dim.x,s.bar_dim.y,BLACK,s.color);
}

void RectTool(void* a,void* b,void* c,void* d){
    pixel* screen = (pixel*)a;
    bool* curdown = (bool*)b;
    ((button*)d)->ispressed = true;
    *curdown = false;
    state = rect;
}

void NewButton(void* a,void* b,void* c,void* d){
    pixel* screen = (pixel*)a;
    char* width = (char*)b;
    char* height = (char*)c;
    CanvasFitDim(screen,atoi(width),atoi(height));
}

void EllipseTool(void* a,void* b,void* c,void* d){
    bool* curdown = (bool*)a;
    ((button*)d)->ispressed = true;
    *curdown = false;
    state = ellipse;
}

void HollowEllipseTool(void* a,void* b,void* c,void* d){
    bool* curdown = (bool*)a;
    ((button*)d)->ispressed = true;
    *curdown = false;
    state = HollowEllipse;
}

void HollowRectTool(void* a,void* b,void* c,void* d){
    bool* curdown = (bool*)a;
    ((button*)d)->ispressed = true;
    *curdown = false;
    state = HollowRect;
}

void LineTool(void* a,void* b,void* c,void* d){
    bool* curdown = (bool*)a;
    ((button*)d)->ispressed = true;
    *curdown = false;
    state = line;
}

void RatioLock(void* a,void* b,void* c,void* d){
    bool* ratiolock = (bool*)a;
    ((button*)d)->ispressed = true;
    if(*ratiolock == true){
        *ratiolock = false;
        ((button*)d)->ispressed = false;
    }
    else{
        *ratiolock = true;
        ((button*)d)->ispressed = true;

    }
}

void Bored(void* a,void* b,void* c,void* d){
    enum Window* w = (enum Window*)a;
    bool* curdown = (bool*)b;
    *curdown = false;
    *w = paint; 
}

void Eraser(void* a,void* b,void* c,void* d){
  
    bool* eraser_on = (void*)a;
    
    if(*eraser_on == false){
        *eraser_on = true;
        pen.color = WHITE;
        ((button*)d)->ispressed = true;
    }
    else{
        *eraser_on = false;
        pen.color = (Color){(int)(255 * gui.sliders[0].slide),(int)(255 * gui.sliders[1].slide),(int)(255 * gui.sliders[2].slide),255};
        ((button*)d)->ispressed = false;
    }
}

void FillBucket(pixel* screen,Vector2 t){
    vector ToBeChecked = GiveVector();
    Color target = screen(t);
    if(ColorComp(pen.color,(screen(t)))) return;
    int index = 0;
    Vector_Add50e(&ToBeChecked,t);
    while (index < ToBeChecked.len){
        Vector2 seed = (ToBeChecked.arr)[index];
        index++;
        if(ColorComp(screen(seed),target)){
           
            screen(seed) = pen.color;
            int leftx = seed.x,rightx = seed.x;
            int i = 1;
            while(ColorComp(screen(((Vector2){seed.x-i,seed.y})),target)
            && seed.x - i > gui.CanvasTopLeft.x && seed.x - i < gui.CanvasBottomRight.x){
                screen(((Vector2){seed.x-i,seed.y})) = pen.color;
                i++;
            }
            leftx-=i;
            i = 1;
            while(ColorComp(screen(((Vector2){seed.x+i,seed.y})),target)
            && seed.x + i > gui.CanvasTopLeft.x && seed.x + i < gui.CanvasBottomRight.x){
                screen(((Vector2){seed.x+i,seed.y})) = pen.color;
                i++;
            }
            rightx+=i;
            for(int t = leftx+1;t < rightx;t++){
            if(seed.y + 1 < gui.CanvasBottomRight.y && ColorComp(screen(((Vector2){t,seed.y+1})),target)){
                Vector_Add50e(&ToBeChecked,(Vector2){t,seed.y+1});
            }
            if(seed.y - 1 > gui.CanvasTopLeft.y && ColorComp(screen(((Vector2){t,seed.y-1})),target)){
                Vector_Add50e(&ToBeChecked,(Vector2){t,seed.y-1});
            }
            }
        }
    }
    free(ToBeChecked.arr);
}

void FillMaskBucket(pixel* screen,pixel* mask,Vector2 t){
    vector ToBeChecked = GiveVector();
    Color target = mask(t);
    if(ColorComp(pen.color,(mask(t)))) return;
    int index = 0;
    Vector_Add50e(&ToBeChecked,t);
    while (index < ToBeChecked.len){
        Vector2 seed = (ToBeChecked.arr)[index];
        index++;
        if(ColorComp(mask(seed),target)){
           
            screen(seed) = pen.color;
            mask(seed) = pen.color;

            int leftx = seed.x,rightx = seed.x;
            int i = 1;
            while(ColorComp(mask(((Vector2){seed.x-i,seed.y})),target)
            && seed.x - i > 0 && seed.x - i < 1280){
                screen(((Vector2){seed.x-i,seed.y})) = pen.color;
                mask(((Vector2){seed.x-i,seed.y})) = pen.color;
                i++;
            }
            leftx-=i;
            i = 1;
            while(ColorComp(mask(((Vector2){seed.x+i,seed.y})),target)
            && seed.x + i > 0 && seed.x + i < 1280){
                mask(((Vector2){seed.x+i,seed.y})) = pen.color;
                screen(((Vector2){seed.x+i,seed.y})) = pen.color;
                i++;
            }
            rightx+=i;
            for(int t = leftx+1;t < rightx;t++){
            if(seed.y + 1 < 720 && ColorComp(mask(((Vector2){t,seed.y+1})),target)){
                Vector_Add50e(&ToBeChecked,(Vector2){t,seed.y+1});
            }
            if(seed.y - 1 > 0 && ColorComp(mask(((Vector2){t,seed.y-1})),target)){
                Vector_Add50e(&ToBeChecked,(Vector2){t,seed.y-1});
            }
            }
        }
    }
    free(ToBeChecked.arr);
    ClearMask(mask);
}

void FillToolActivation(void* a,void* b,void* c,void* d){
    bool* curdown = (bool*)a;
    *curdown = false;
    state = Bucket;
    ((button*)d)->ispressed = true;
}

void EyeDropper(void* a,void* b,void* c,void*d){
    bool* curdown = (bool*)a;
    *curdown = false;
    state = eyedropper;
    ((button*)d)->ispressed = true;
}

void LoadReference(void* a,void* b,void* c,void* d){
    Texture2D* ref = (Texture2D*)a; 
    bool* ref_on = (bool*)b;
    Rectangle* reference_destrect = (Rectangle*)c;
    Vector2* reference_origin = (Vector2*)d;
    if(*ref_on){
        *ref_on = false;
        *reference_destrect = (Rectangle){800,400,ref->width,ref->height};
        *reference_origin = (Vector2){ref->width/2,ref->height/2};
        UnloadTexture(*ref);
    }
    else{
        *ref_on = true;
        *ref = LoadTexture(gui.textboxes[5].text);
        *reference_destrect = (Rectangle){800,400,ref->width,ref->height};
        *reference_origin = (Vector2){ref->width/2,ref->height/2};
    }
}

void MoveReference(void* a,void* b,void* c,void* d){
    bool* curdown = (bool*)a;
    *curdown = false;
    state = moveref; 
    ((button*)d)->ispressed = true;

    Rectangle* reference_destrect = (Rectangle*) c;
    *((Vector2*)b) =  Vector2Minus((Vector2){reference_destrect->x,reference_destrect->y},GetMousePosition());

}

void ChangeBrushCircle(void*a , void* b,void* c,void* d){
    bool* curdown = (bool*)a;
    *curdown = false;
    pen.brush = circle;
}

void ChangeBrushSquare(void*a , void* b,void* c,void* d){
    bool* curdown = (bool*)a;
    *curdown = false;
    pen.brush = square;
}

void ChangeBrushSpray(void*a , void* b,void* c,void* d){
    bool* curdown = (bool*)a;
    *curdown = false;
    pen.brush = spray;
}








