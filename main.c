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
  bool mouseIsDown = false;
  // int tools[] = {1,2,3};

  RenderTexture2D target = LoadRenderTexture(1000,1000);
  RenderTexture2D target2 = LoadRenderTexture(1000,1000);

  // Clear render texture before entering the game loop
  // BeginTextureMode(target);
  // // ClearBackground(BLACK);
  // EndTextureMode();

  // SetTargetFPS(1200000000000000); 
  
  Vector2 prevMouse = {0};
  bool penStarted = false;
  Vector2 pm = {0};
    
  while(!WindowShouldClose()) {
      
    // tools = CIRCLE;
    if(IsKeyPressed(KEY_R)){
      tools = RECTANGLE;
    } else if(IsKeyPressed(KEY_C)){
      tools = CIRCLE;
    } else if(IsKeyPressed(KEY_P)){
      tools = PEN;
    } else if (IsKeyPressed(KEY_L)){
      tools = LINE;
    }
    if(IsKeyPressed(KEY_A)){
      BeginTextureMode(target);
      ClearBackground(BLACK);
      EndTextureMode();
    }
    // int xpen;
    // int ypen;

    // int x = GetMouseX();
    // int y = GetMouseY();
    // mouseWasPressed = false;
    // bool penfirstcircle = false;
    // if(IsMouseButtonDown(MOUSE_BUTTON_LEFT) && tools == PEN && mouseWasPressed == false ){
    //   // mouseWasPressed = true;
    //   xpen= GetMouseX();
    //   ypen= GetMouseY();

    //   if(penfirstcircle){

    //   }
      
    //   Vector2 del = GetMouseDelta();

    //   BeginTextureMode(target);
    //   DrawCircle(del.x+100,del.y+100,10,RED);

    //   DrawCircle(x,y,10,GREEN);

    //   DrawRectangle(10,10,100,100,BLACK);
    //   DrawFPS(10,10);
            
    //   EndTextureMode();
      
    //   // DrawFPS(10,10);
    // } 
    // else if(IsMouseButtonReleased(MOUSE_BUTTON_LEFT) && tools == PEN ){
    //   // mouseWasPressed = false;
    //     BeginTextureMode(target);
    //   DrawCircle(10,10,100,GREEN);

    //   // DrawRectangle(10,10,100,100,BLACK);
    //   // DrawFPS(10,10);
            
    //   EndTextureMode();
    // }

    Vector2 curr = GetMousePosition();

    if (IsMouseButtonDown(MOUSE_LEFT_BUTTON) && tools == PEN)
    {
      if (!penStarted)
      {
          prevMouse = curr;
          penStarted = true;
      }

      float dx = curr.x - prevMouse.x;
      float dy = curr.y - prevMouse.y;
      float dist = sqrt(dx*dx + dy*dy);

      float spacing = 5.0f;

      BeginTextureMode(target);

      for (float i = 0; i < dist; i += spacing)
      {
          float t = i / dist;

          float x = prevMouse.x + dx * t;
          float y = prevMouse.y + dy * t;

          DrawCircle(x, y, 10, RED);
        }
        
        DrawRectangle(10,10,100,100,BLACK);
  
        DrawText(TextFormat("%d a",curr),10,10,20,RED);
        DrawText(TextFormat("%d bv",prevMouse),20,30,20,RED);
      EndTextureMode();
      
      prevMouse = curr;
    }
    else if (IsMouseButtonReleased(MOUSE_LEFT_BUTTON) && tools == PEN)
    {
        penStarted = false;
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
    // int xlinerec;
    // int ylinerec;
    // mouseWasPressed = true;
    // if(IsMouseButtonPressed(MOUSE_BUTTON_LEFT) && tools == RECTANGLE && mouseWasPressed == false) {
    //   mouseWasPressed = true;
    //   xlinerec= GetMouseX();
    //   ylinerec= GetMouseY();
    // } else if(IsMouseButtonPressed(MOUSE_BUTTON_LEFT) && tools == RECTANGLE && mouseWasPressed) {
    //   mouseWasPressed = false;
    //   int x= GetMouseX();
    //   int y= GetMouseY();
    //   BeginTextureMode(target);
    //   DrawRectangleLines(xlinerec,ylinerec,x-xlinerec,y-ylinerec,BLUE);
    //   DrawCircle((xlinerec+x)/2,(ylinerec+y)/2,10,RED);
      
    //   DrawRectangle(10,10,100,100,BLACK);
    //   DrawFPS(10,10);
    //   EndTextureMode();
      
    // }
    Vector2 a;

    if(IsMouseButtonDown(MOUSE_BUTTON_LEFT) && tools == RECTANGLE) {
      if(!mouseIsDown){
        a=GetMousePosition();
        pm = a;
        mouseIsDown = true;
        continue;
      }
      // if (a.x)
      // {
      //   a=GetMousePosition();
      // }
      
      
      a=GetMousePosition();
      int x= GetMouseX();
      int y= GetMouseY();
      BeginTextureMode(target2);
      ClearBackground(BLACK);
      DrawRectangleLines(pm.x,pm.y,a.x-pm.x,a.y-pm.y,BLUE);
      // DrawRectangle(pm.x+1,pm.y+1,a.x-pm.x,a.y-pm.y,RED);
      // DrawCircle((pm.x+x)/2,(pm.y+y)/2,10,RED);
      
      DrawRectangle(10,10,100,100,BLACK);
      DrawFPS(10,10);
      EndTextureMode();
      
      // mouseIsDown = false;
      
      // a=GetMousePosition();
    }
    else if(IsMouseButtonReleased(MOUSE_BUTTON_LEFT) && tools == RECTANGLE){

      mouseIsDown = false;
      BeginTextureMode(target);
      // ClearBackground(BLACK);
      DrawRectangleLines(pm.x,pm.y,a.x-pm.x,a.y-pm.y,BLUE);
      // DrawRectangle(pm.x+1,pm.y+1,a.x-pm.x,a.y-pm.y,RED);
      // DrawCircle((pm.x+x)/2,(pm.y+y)/2,10,RED);
      
      DrawRectangle(10,10,100,100,BLACK);
      DrawFPS(10,10);
      EndTextureMode();
    }
    int xlinecircle;
    int ylinecircle;
    if(IsMouseButtonDown(MOUSE_BUTTON_LEFT) && tools == CIRCLE) {
      // mouseWasPressed = true;
      xlinecircle= GetMouseX();
      ylinecircle= GetMouseY();

      if(!mouseIsDown){
        a=GetMousePosition();
        pm = a;
        mouseIsDown = true;
        continue;
      }
      // if (a.x)
      // {
      //   a=GetMousePosition();
      // }
      
      
      a=GetMousePosition();
      int x= GetMouseX();
      int y= GetMouseY();
      BeginTextureMode(target2);
      ClearBackground(BLACK);
      float dx = a.x-pm.x;
      float dy = a.y -pm.y;
      float radius = sqrt(dx*dx + dy*dy);
      
      DrawCircleLines((pm.x+a.x)/2,(pm.y+a.y)/2,radius/2,PINK);
      
      DrawRectangle(10,10,100,100,BLACK);
      DrawFPS(10,10);
      EndTextureMode();
    } else if(IsMouseButtonReleased(MOUSE_BUTTON_LEFT) && tools == CIRCLE) {
      // mouseWasPressed = false;
      int x= GetMouseX();
      int y= GetMouseY();
      BeginTextureMode(target);
      ClearBackground(BLACK);
      float dx = a.x-pm.x;
      float dy = a.y -pm.y;
      float radius = sqrt(dx*dx + dy*dy);
      
      DrawCircleLines((pm.x+a.x)/2,(pm.y+a.y)/2,radius/2,PINK);
      
      DrawRectangle(10,10,100,100,BLACK);
      DrawFPS(10,10);
      EndTextureMode();
      
    }
    BeginDrawing();
    // ClearBackground(RAYWHITE);
    DrawTextureRec(target2.texture, (Rectangle){ 0, 0, (float)target2.texture.width, (float)-target2.texture.height }, (Vector2) { 0, 0 }, WHITE);
    DrawTextureRec(target.texture, (Rectangle){ 0, 0, (float)target.texture.width, (float)-target.texture.height }, (Vector2) { 0, 0 }, WHITE);
    EndDrawing();
  }
    
  CloseWindow();
    
  return 0;
}