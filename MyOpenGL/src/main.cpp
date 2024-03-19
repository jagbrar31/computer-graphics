/*
 * Authors: Jagpreet Brar & Daman Rao
 * Description: 3D snake game
 *
 */


#include <stdio.h>
#include <time.h>
#include <windows.h>
#include <gl/glut.h>

#define UP 1
#define Down 2
#define LEFT 3
#define RIGHT 4
//variables
//Screen
GLint   w       = 800;
GLint   h       = 550;
GLint   Giw     = 0;
GLint   Gih     = 0;
GLint   Gfw     = 150;
GLint   Gfh     = 150;

//Snake
GLint   bodyPos[2][100] = {{}};
GLint   sx       = 5;
GLint   sz       = 10;
GLint   oldX[2] = {};
GLint   oldZ[2] = {};
GLbyte  direction  = 0;

//Status
GLint   point = 0;
GLint   size  = 0;
GLbyte  Over = true;
GLbyte  EnableLight = true;
GLbyte  played = false;

//Food
GLint   fx      = 0;
GLint   fz      = 0;

//Camera Angle
static GLfloat rotx=45.0F ;
static GLfloat roty=0.0F ;
static GLfloat rotz=0.0F ;

static GLfloat rotHead=90.0F ;
static GLfloat zoom=-300.0f;

//initialize the first configurations
void Initialize(void)
{


    glEnable(GL_DEPTH_TEST);
    glClearColor(0.5f, 0.5f, 1.0f,0.0); //Change the background color
    if(EnableLight){
        glEnable(GL_COLOR_MATERIAL); //Enable color
        glEnable(GL_LIGHTING); //Enable lighting
        glEnable(GL_LIGHT0); //Enable light #0
        glEnable(GL_LIGHT1); //Enable light #1
        glEnable(GL_NORMALIZE); //Automatically normalize normals
    }
}

//Configure the lightning
void initLight()
{
   //Add ambient light
    GLfloat ambientColor[] = {0.3f, 0.4f, 0.8f, 1.0f};
    glLightModelfv(GL_LIGHT_MODEL_AMBIENT, ambientColor);
    //Add positioned light
    GLfloat lightColor0[] = {0.5f, 0.5f, 0.5f, 1.0f};
    GLfloat lightPos0[] = {4.0f, 0.0f, 8.0f, 1.0f};
    glLightfv(GL_LIGHT0, GL_DIFFUSE, lightColor0);
    glLightfv(GL_LIGHT0, GL_POSITION, lightPos0);
    //Add directed light
    GLfloat lightColor1[] = {0.5f, 0.2f, 0.2f, 1.0f};
    //Coming from the direction (-1, 0.5, 0.5)
    GLfloat lightPos1[] = {-1.0f, 0.5f, 0.5f, 0.0f};
    glLightfv(GL_LIGHT1, GL_DIFFUSE, lightColor1);
    glLightfv(GL_LIGHT1, GL_POSITION, lightPos1);
}

//Configure window resize
void resize (int w, int h)
{
    glViewport(0, 0, w, h);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(45.0, (double)w / (double)h,1, 800.0);
}
void Write(char *string){//Write string on the screen
    while(*string)
        glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, *string++);
}
//This Function will rotate the object according to the Angles
void ManipulateViewAngle() {
    glRotatef(rotx,1.0,0.0,0.0);//Rotate Arround X axis
    glRotatef(roty,0.0,1.0,0.0);//Rotate Arround Y axis
    glRotatef(rotz,0.0,0.0,1.0);//Rotate Arround Z axis
}
//Display a welcome screen or end screen
void WelcomeScreen(){
    char tmp_str[40];



    if(played){
    	glRasterPos2f(-50, 0);
        Write("Game over! Press 'G' or 'g' to play again (esc to exit)");
    	glRasterPos2f(-50, 20);
        sprintf(tmp_str, "point: %d",  point);
        Write(tmp_str);
    }else{
    	glRasterPos2f(-40, 20);
    	Write("Welcome to the 3D snake game");
    	glRasterPos2f(-40, 0);
    	Write("Press 'G' or 'g' to play ");

    }

}
//This Function will reset the snake size and location...
void Reset(){
    sx       = 5;
    sz       = 10;
    direction  = 0;
    point = 0;
    size  = 0;
    Over = false;
    rotx=45.0F ;
    roty=0.0F ;
    rotz=0.0F ;
    rotHead=90.0F ;
}

void DrawSnake(){
    int  i;
    //Drawing the head & the plane
    glPushMatrix();
    ManipulateViewAngle();
        //This will draw the plane that the snake will run on.
        glPushMatrix();
        glColor3f(0.5, 0.35, 0.05);
        //glColor3f(0.0, 0.6, 0.2);
        glTranslatef(75.0, -16.0, 75.0);
        glScalef(155,5.0,155);
        glutSolidCube(1);
        glPopMatrix();
    //Here we will draw the Head of the snake
    glColor3f(0,1,0);//Color it green
    glTranslatef(sx,-10.0,sz);//Give it the location according to sx & sz
    glScalef(0.5,0.5,0.5);
    glutSolidSphere(10,20,20);//Draw the head as a sphere
    glRotatef(rotHead, 0.0, 1.0, 0.0);
    glColor3f(0,1,0);
    glTranslatef(0,0.0,6.0);
    glScalef(0.8,1.0,1.0);
    glutSolidCone(10,10,20,20);
    glColor3f(1,1,1);
    glTranslatef(-4.0,10.0,0.0);
    glScalef(0.3,0.3,0.3);
    glutSolidSphere(10,20,20);
    glTranslatef(26.0,0.0,0.0);
    glutSolidSphere(10,20,20);
    glPopMatrix();

    //Drawing the body
    for(i=0; i<size; i++){//Loop throw the size and draw spheres representing the body
        glPushMatrix();
        ManipulateViewAngle();
        glTranslatef(bodyPos[0][i],-10.0,bodyPos[1][i]);//this will locate the spheres
        glColor3f(0,1,0);//Color green
        glScalef(0.5,0.5,0.5);
        glutSolidSphere(7,20,20);
        glPopMatrix();
    }
}
void GameStatus(){
    char tmp_str[40];
    glColor3f(0.8, 0.2, 0);
    glRasterPos2f(5, 20);
    sprintf(tmp_str, "3D Snake Game");
    Write(tmp_str);
    // Print the status of the game on the screen
    glColor3f(0, 0, 0);
    glRasterPos2f(5, 10);
    sprintf(tmp_str, "point: %d",  point);
    Write(tmp_str);
}
void DrawFood()
{

    //Draw the Sphere representing the Food for the snake
    glPushMatrix();
    ManipulateViewAngle();
    glTranslatef(fx,-10.0,fz);
    glColor3f(1, 1, 0);
    glEnable(GL_TEXTURE_2D);
    //glBindTexture(GL_TEXTURE_2D,1);
    glScalef(0.5,0.5,0.5);
    glutSolidSphere(7,20,20);
    glPopMatrix();
}

//Generates Random Numbers for the location of the food that the snake will eat
int RandomNumber(int high, int low)
{
    return (rand() % (high-low))+low;
}
//This Function Will Check for Collision
bool collision(){
    int i;
    for(i=0; i<size; i++){
        if((bodyPos[0][i] == sx && bodyPos[1][i] == sz) ||
        //((bodyPos[0][i] >= sx) && (bodyPos[0][i] <= sx + 5) && (bodyPos[1][i] >= sz) && (bodyPos[1][i] <= sz + 5)) ||
        //((bodyPos[0][i] <= sx) && (bodyPos[0][i] >= sx - 5) && (bodyPos[1][i] <= sz) && (bodyPos[1][i] >= sz - 5)) ||
        //((bodyPos[0][i] <= sx) && (bodyPos[0][i] >= sx - 5) && (bodyPos[1][i] >= sz) && (bodyPos[1][i] <= sz + 5)) ||
        ((bodyPos[0][i] >= sx) && (bodyPos[0][i] <= sx + 5) && (bodyPos[1][i] <= sz) && (bodyPos[1][i] >= sz - 5)))
        return true;
    }
   return false;
}
//Generate the New Food that the snake will eat
void newFood(){
    time_t seconds;
    time(&seconds);
    srand((unsigned int) seconds);
    fx = RandomNumber(Gfw-Giw, Giw+10);
    fz = RandomNumber(Gfh-Gih, Gih+10);
}
//This Function will move the snake according to the directions
//Taken from the Keyboard keys
void Run(int value){
    int i;
   oldX[1] = sx;
    oldZ[1] = sz;
    switch(direction){
      case RIGHT:
          rotHead =90;
          sx += 6;
          if(sx > Gfw-2) sx = Giw-1;//This will check if the snake is going into the border so it will appear on the other side
          break;
      case LEFT:
          rotHead =-90;
          sx -= 6;
         if(sx < 0) sx = Gfw-2;//This will check if the snake is going into the border so it will appear on the other side
          break;
      case UP:
          rotHead =0;
          sz += 6;
          if(sz > Gfh-2) sz = Gih-1;//This will check if the snake is going into the border so it will appear on the other side
          break;
      case Down:
          rotHead =180;
          sz -= 6;
          if(sz < 2) sz = Gfh-2;//This will check if the snake is going into the border so it will appear on the other side
          break;
    }
    //Checks for Collisoin if yes Game Over
    if(collision()){
    	played = true;
    	Over = true;
    }
    //Checks if the snake ate the food (check the X and Y)
    // If yes it will increase the point & the size of the snake & create a new food
    if((sx == fx && sz == fz) ||
    ((sx >= fx) && (sx <= fx + 4) && (sz >= fz) && (sz <= fz + 4)) ||
    ((sx <= fx) && (sx >= fx - 4) && (sz <= fz) && (sz >= fz - 4)) ||
    ((sx <= fx) && (sx >= fx - 4) && (sz >= fz) && (sz <= fz + 4)) ||
    ((sx >= fx) && (sx <= fx + 4) && (sz <= fz) && (sz >= fz - 4))){
        point++;
        if(point < 100) size++;
        newFood();
    }

    for(i = 0; i<size; i++){// Save the positions of the body parts
    	oldX[0]=oldX[1];
        oldZ[0]= oldZ[1];
        oldX[1] = bodyPos[0][i];
        oldZ[1] = bodyPos[1][i];
        bodyPos[0][i] =oldX[0];
        bodyPos[1][i] = oldZ[0];
    }

    //Set the Timer
    glutTimerFunc(130, Run, 0);
}

void Display(void){//Draw Function
    // Clear screen
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    if(EnableLight) initLight();

    glTranslatef (-60.0, 40.0, zoom);
    //Check if the value of the Flag "Game Over is not True it will continue the game"

    if(!Over){
        GameStatus();
        DrawFood();
        DrawSnake();
    }
    else
        WelcomeScreen();
    // Updates the screen
    glutPostRedisplay();
    glutSwapBuffers();
}
void Special(int key, int x, int y){
    switch(key){
    case GLUT_KEY_RIGHT     :
        if(direction != LEFT)
            direction = RIGHT;
        break;
    case GLUT_KEY_LEFT      :
        if(direction != RIGHT)
            direction = LEFT;
        break;
    case GLUT_KEY_UP        :
        if(direction != UP)
            direction = Down;
        break;
    case GLUT_KEY_DOWN      :
        if(direction != Down)
            direction = UP;
        break;
    }
}
void keyboard (unsigned char key, int x, int y)
{
    switch (key) {
      case 'G'   : Reset() ;
          glutPostRedisplay();
          break;
    case 'g'    : Reset() ;
          glutPostRedisplay();
          break;
      //ESC to Exit
      case 27:
          exit(0);
          break;
      default:
          break;
    }
}
int main(int argc, char** argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
    glutInitWindowSize(w,h);
    glutInitWindowPosition(80,80);
    glutCreateWindow("Snake Game");
    Initialize();
    glutSpecialFunc(Special);
    glutKeyboardFunc(keyboard);
    glutDisplayFunc(Display);
    glutReshapeFunc(resize);
    newFood();
    Run(0);
    glutMainLoop();
}
