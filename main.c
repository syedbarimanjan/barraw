#include <stdlib.h>
#include <time.h>

#include <raylib.h>
#include <raymath.h>

typedef enum Tools {
  PEN,
  LINE,
} Tools;

Tools tools;

int main() {
  InitWindow(1000, 1000, "Barraw");
  bool mouseWasPressed = false;
  // int tools[] = {1,2,3};

  while(!WindowShouldClose()) {

    tools = LINE;
    // DrawRectangle(10,10,100,40,RAYWHITE);

    
    // for (int i = 0; i < 3; i++)
    // {
    //   DrawRectangleLines(10,10,30,40,GREEN);
    // }
    


    
    // ClearBackground(RAYWHITE);
    // SetTargetFPS(144);
    if(IsMouseButtonDown(MOUSE_BUTTON_LEFT) && tools == PEN && mouseWasPressed == false ){
      mouseWasPressed = true;
      int x= GetMouseX();
      int y= GetMouseY();
      
      DrawCircle(x,y,10,RED);
      // DrawFPS(10,10);
    } 

    int xline;
    int yline;
    if(IsMouseButtonPressed(MOUSE_BUTTON_LEFT) && tools == LINE && mouseWasPressed == false) {
      mouseWasPressed = true;
      xline= GetMouseX();
      yline= GetMouseY();
      
      // Vector2 xx = GetMouseDelta();
      
    } else if(IsMouseButtonPressed(MOUSE_BUTTON_LEFT) && tools == LINE && mouseWasPressed) {
      mouseWasPressed = false;
      int x= GetMouseX();
      int y= GetMouseY();
      DrawLine(xline,yline,x,y,RED);
      
    }


    // if(IsMouseButtonReleased(MOUSE_BUTTON_LEFT)){
    //   DrawRectangle(10,10,100,100,RED);
    // }
    BeginDrawing();
      // else if (IsMouseButtonUp(MOUSE_BUTTON_LEFT)) {
      //   int x= GetMouseX();
      //   int y= GetMouseY();
        
      //   DrawCircle(x,y,10,BLACK);
      // }

      // if (IsMouseButtonReleased(MOUSE_BUTTON_LEFT))
      // {
      //   int x= GetMouseX();
      //   int y= GetMouseY();
      //   Vector2 mousepos2 = GetMousePosition();
      //   DrawText("10",mousepos2.x,mousepos2.y,100,BLACK);

        
      //   DrawCircle(x,y,10,BLACK);
      // }
      // if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT))
      // {
      //   int x= GetMouseX();
      //   int y= GetMouseY();
      //   Vector2 mousepos2 = GetMousePosition();
      //   DrawText("10",mousepos2.x,mousepos2.y,100,BLACK);
        
      //   DrawCircle(x,y,10,BLACK);
      // }

      // if (IsMouseButtonUp(MOUSE_BUTTON_LEFT))
      // {
      //   int x= GetMouseX();
      //   int y= GetMouseY();
      //   DrawText("10",10,10,100,BLACK);

        
      //   DrawCircle(x,y,10,WHITE);
      // }
    EndDrawing();
  } 
    
  CloseWindow();
    
  return 0;
}