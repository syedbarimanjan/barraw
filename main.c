#include <stdlib.h>
#include <time.h>

#include <raylib.h>
#include <raymath.h>

typedef enum Tools {
  PEN,
  LINE,
  RECTANGLE,
  CIRCLE,
} Tools;

Tools tools;

int main() {
  InitWindow(1000, 1000, "Barraw");
  bool mouseWasPressed = false;
  // int tools[] = {1,2,3};

  RenderTexture2D target = LoadRenderTexture(1000,1000);

    // Clear render texture before entering the game loop
    // BeginTextureMode(target);
    // // ClearBackground(BLACK);
    // EndTextureMode();

    // SetTargetFPS(1200000000000000);              // Set our game to run at 120 frames-per-second

  while(!WindowShouldClose()) {

    tools = RECTANGLE;
    if(IsMouseButtonDown(MOUSE_BUTTON_LEFT) && tools == PEN && mouseWasPressed == false ){
      // mouseWasPressed = true;
      int x= GetMouseX();
      int y= GetMouseY();

      BeginTextureMode(target);
      DrawCircle(x,y,10,RED);

      DrawRectangle(10,10,100,100,BLACK);
      DrawFPS(10,10);
            
      EndTextureMode();
      
      // DrawFPS(10,10);
    } 

    int xline;
    int yline;
    if(IsMouseButtonPressed(MOUSE_BUTTON_LEFT) && tools == LINE && mouseWasPressed == false) {
      mouseWasPressed = true;
      xline= GetMouseX();
      yline= GetMouseY();
    } else if(IsMouseButtonPressed(MOUSE_BUTTON_LEFT) && tools == LINE && mouseWasPressed) {
      mouseWasPressed = false;
      int x= GetMouseX();
      int y= GetMouseY();
      BeginTextureMode(target);
      DrawLine(xline,yline,x,y,GREEN);
      
      DrawRectangle(10,10,100,100,BLACK);
      DrawFPS(10,10);
      EndTextureMode();
      
    }
    int xlinerec;
    int ylinerec;
    if(IsMouseButtonPressed(MOUSE_BUTTON_LEFT) && tools == RECTANGLE && mouseWasPressed == false) {
      mouseWasPressed = true;
      xlinerec= GetMouseX();
      ylinerec= GetMouseY();
    } else if(IsMouseButtonPressed(MOUSE_BUTTON_LEFT) && tools == RECTANGLE && mouseWasPressed) {
      mouseWasPressed = false;
      int x= GetMouseX();
      int y= GetMouseY();
      BeginTextureMode(target);
      DrawRectangleLines(xlinerec,ylinerec,x-xlinerec,y-ylinerec,BLUE);
      DrawCircle((xlinerec+x)/2,(ylinerec+y)/2,10,RED);
      
      DrawRectangle(10,10,100,100,BLACK);
      DrawFPS(10,10);
      EndTextureMode();
      
    }
    int xlinecircle;
    int ylinecircle;
    if(IsMouseButtonPressed(MOUSE_BUTTON_LEFT) && tools == CIRCLE && mouseWasPressed == false) {
      mouseWasPressed = true;
      xlinecircle= GetMouseX();
      ylinecircle= GetMouseY();
    } else if(IsMouseButtonPressed(MOUSE_BUTTON_LEFT) && tools == CIRCLE && mouseWasPressed) {
      mouseWasPressed = false;
      int x= GetMouseX();
      int y= GetMouseY();
      BeginTextureMode(target);

      float dx = x-xlinecircle;
      float dy = y -ylinecircle;
      float radius = sqrt(dx*dx + dy*dy);
      
      DrawCircleLines((xlinecircle+x)/2,(ylinecircle+y)/2,radius/2,PINK);
      
      DrawRectangle(10,10,100,100,BLACK);
      DrawFPS(10,10);
      EndTextureMode();
      
    }
    BeginDrawing();
    // ClearBackground(RAYWHITE);
    DrawTextureRec(target.texture, (Rectangle){ 0, 0, (float)target.texture.width, (float)-target.texture.height }, (Vector2) { 0, 0 }, WHITE);
    EndDrawing();
  } 
    
  CloseWindow();
    
  return 0;
}