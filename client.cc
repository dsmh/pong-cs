
////////////////////////////////////////////////////////////
// Headers
////////////////////////////////////////////////////////////
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <cmath>
#include <ctime>
#include <cstdlib>
#include <czmq.h>
#include <string>
#include <stdio.h>
#include <unistd.h>
#include <iostream>
#include <sstream>
#include <vector>
#include <map>


using namespace std;
using std::cout;
using std::cerr;
using std::endl;
using std::vector;
using std::string;

void sendMsg(void* channel, vector<string> parts) {
  zmsg_t* msg = zmsg_new();
  for (const string& s : parts) {
    zmsg_addstr(msg, s.c_str());
  }
  zmsg_send(&msg, channel);
}

string intToS(int n)
{
  std::ostringstream sin;
  sin << n;
  std::string val = sin.str();
  return val;
}

// Some game cosntants
const sf::Vector2f paddleSize(25, 100);
const sf::Vector2f paddleSizeh(100, 25);
const float pi = 3.14159f;
const float paddleSpeed = 700.f;
const int gameWidth = 500;
const int gameHeight = 500;
const float ballRadius = 10.f;

 
sf::RectangleShape createUpperPaddle() {
  sf::RectangleShape upperPaddle;
  upperPaddle.setSize(paddleSizeh - sf::Vector2f(3, 3));
  upperPaddle.setOutlineThickness(3);
  upperPaddle.setOutlineColor(sf::Color::Black);
  upperPaddle.setFillColor(sf::Color(0, 100, 200));
  upperPaddle.setOrigin(paddleSizeh / 2.f);
  return upperPaddle;
}

sf::RectangleShape createDownPaddle() {
  sf::RectangleShape DownPaddle;
  DownPaddle.setSize(paddleSizeh - sf::Vector2f(3, 3));
  DownPaddle.setOutlineThickness(3);
  DownPaddle.setOutlineColor(sf::Color::Black);
  DownPaddle.setFillColor(sf::Color(200, 100, 0));
  DownPaddle.setOrigin(paddleSizeh / 2.f);
  return DownPaddle;
}

sf::RectangleShape createLeftPaddle() {
  sf::RectangleShape leftPaddle;
  leftPaddle.setSize(paddleSize - sf::Vector2f(3, 3));
  leftPaddle.setOutlineThickness(3);
  leftPaddle.setOutlineColor(sf::Color::Black);
  leftPaddle.setFillColor(sf::Color(100, 100, 200));
  leftPaddle.setOrigin(paddleSize / 2.f);
  return leftPaddle;
}

sf::RectangleShape createRightPaddle() {
  sf::RectangleShape rightPaddle;
  rightPaddle.setSize(paddleSize - sf::Vector2f(3, 3));
  rightPaddle.setOutlineThickness(3);
  rightPaddle.setOutlineColor(sf::Color::Black);
  rightPaddle.setFillColor(sf::Color(200, 100, 100));
  rightPaddle.setOrigin(paddleSize / 2.f);
  return rightPaddle;
}

sf::CircleShape createBall(void) {
  sf::CircleShape ball;
  ball.setRadius(ballRadius - 3);
  ball.setOutlineThickness(3);
  ball.setOutlineColor(sf::Color::Black);
  ball.setFillColor(sf::Color::White);
  ball.setOrigin(ballRadius / 2, ballRadius / 2);
  return ball;
}

bool checkTermination(const sf::Event& event) {
  return (event.type == sf::Event::Closed) ||
         ((event.type == sf::Event::KeyPressed) &&
          (event.key.code == sf::Keyboard::Escape));
}

void startScene(sf::RectangleShape& leftPaddle, sf::RectangleShape& rightPaddle,sf::RectangleShape& upperPaddle, sf::RectangleShape& downPaddle, sf::CircleShape& ball, float& ballAngle) {
  // Reset the position of the paddles and ball
  upperPaddle.setPosition(gameWidth/2- paddleSizeh.x / 2, (paddleSizeh.y/2)+5);
  downPaddle.setPosition(gameWidth/2- paddleSizeh.x / 2, gameHeight-paddleSizeh.y/2);
  leftPaddle.setPosition(5 + paddleSize.x / 2, gameHeight / 2);
  rightPaddle.setPosition(gameWidth - paddleSize.x / 2, gameHeight / 2);
  ball.setPosition(gameWidth / 2, gameHeight / 2);

  // Reset the ball angle
  do {
    // Make sure the ball initial angle is not too much vertical
    ballAngle = (rand() % 360) * 2 * pi / 360;
  } while (std::abs(std::cos(ballAngle)) < 0.7f);
}





void movePlayer1Paddle(sf::RectangleShape& paddle, float deltaTime, void* client,string myName) {


  // Move the player's paddle
  if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up) &&
      (paddle.getPosition().y - paddleSize.y / 2 > 40.f)) {

             sendMsg(client,{"move", myName ,intToS(paddle.getPosition().x) ,intToS(paddle.getPosition().y)});

            //cout << paddle.getPosition().y<<endl;

           paddle.move(0.f, -paddleSpeed * deltaTime);
  }
  if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down) &&
      (paddle.getPosition().y + paddleSize.y / 2 < gameHeight - 40.f)) {
          
             ///sendData(paddle.getPosition().y,socket);
            sendMsg(client,{"move", myName, intToS(paddle.getPosition().x) ,intToS(paddle.getPosition().y)});
            //cout << paddle.getPosition().y<<endl;

    paddle.move(0.f, paddleSpeed * deltaTime);
  }
}

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////


void movePlayer1PaddleHorizontal(sf::RectangleShape& paddle, float deltaTime, void* client, string myName) {


  // Move the player's paddle
  if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left) &&
      (paddle.getPosition().x - paddleSizeh.x/2 > 40.f)) {

 ///sendData(paddle.getPosition().y,socket);
            sendMsg(client,{"move", myName ,intToS(paddle.getPosition().x) ,intToS(paddle.getPosition().y)});
            //cout << paddle.getPosition().y<<endl;

           paddle.move(-paddleSpeed * deltaTime, 0.f);
  }
  if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right) &&
      (paddle.getPosition().x + paddleSizeh.x/2 < gameWidth - 40.f)) {
          
             sendMsg(client,{"move", myName ,intToS(paddle.getPosition().x) ,intToS(paddle.getPosition().y)});

            //cout << paddle.getPosition().y<<endl;

    paddle.move(paddleSpeed * deltaTime, 0.f);
  }
}

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////

void moveBall(sf::CircleShape& ball, float ballSpeed, float ballAngle,
              float deltaTime) {
  float factor = ballSpeed * deltaTime;
  ball.move(std::cos(ballAngle) * factor, std::sin(ballAngle) * factor);
}

bool checkScore(sf::CircleShape& ball) {
  // Check collisions between the ball and the screen
  if (ball.getPosition().x - ballRadius < 0.f) 
    return true;
  if (ball.getPosition().x + ballRadius > gameWidth)
   return true;
  if(ball.getPosition().y + ballRadius > gameHeight)
   return true;
  if(ball.getPosition().y - ballRadius < 0.f)
   return true;

  return false;
}

void checkLateralCollision(sf::CircleShape& ball, float& ballAngle) {
  if (ball.getPosition().y - ballRadius < 0.f) {
    ballAngle = -ballAngle;
    ball.setPosition(ball.getPosition().x, ballRadius + 0.1f);
  }
  if (ball.getPosition().y + ballRadius > gameHeight) {
    ballAngle = -ballAngle;
    ball.setPosition(ball.getPosition().x, gameHeight - ballRadius - 0.1f);
  }
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////// ARREGLAR COLLITIONS./////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////


void checkUpperPaddleCollision(sf::RectangleShape& UpperPaddle, sf::CircleShape& ball, float& ballAngle) {
  if (ball.getPosition().y - ballRadius <
          UpperPaddle.getPosition().y + paddleSizeh.y / 2 &&
      ball.getPosition().y - ballRadius > UpperPaddle.getPosition().y &&
      ball.getPosition().x + ballRadius >=
          UpperPaddle.getPosition().x - paddleSizeh.x / 2 &&
      ball.getPosition().x - ballRadius <=
          UpperPaddle.getPosition().x + paddleSizeh.x / 2) {
    
    if (ball.getPosition().x > UpperPaddle.getPosition().x)
      //ballAngle = pi - ballAngle + (std::rand() % 20) * pi / 180;
      ballAngle = 225;
    else
      //ballAngle = pi - ballAngle - (std::rand() % 20) * pi / 180;
      ballAngle = 315;
    ball.setPosition(UpperPaddle.getPosition().x + ballRadius +
                         paddleSizeh.x / 2 + 0.1f,
                     ball.getPosition().y);
  }

}


void checkLowerPaddleCollision(sf::RectangleShape& lowerPaddle, sf::CircleShape& ball, float& ballAngle) {
  if (ball.getPosition().y + ballRadius >
          lowerPaddle.getPosition().y - paddleSizeh.y / 2 &&
      ball.getPosition().y + ballRadius > lowerPaddle.getPosition().y &&
      ball.getPosition().x + ballRadius >=
          lowerPaddle.getPosition(
            ).x - paddleSizeh.x / 2 &&
      ball.getPosition().x + ballRadius <=
          lowerPaddle.getPosition().x + paddleSizeh.x / 2) {

    if (ball.getPosition().x > lowerPaddle.getPosition().x)
      //ballAngle = pi - ballAngle + (std::rand() % 20) * pi / 180;
      ballAngle = 45;
    else
      //ballAngle = pi - ballAngle - (std::rand() % 20) * pi / 180;
      ballAngle = 135;
    ball.setPosition(lowerPaddle.getPosition().x + ballRadius +
                         paddleSizeh.x / 2 + 0.1f,
                     ball.getPosition().y);
  }

}

////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////



void checkLeftPaddleCollision(sf::RectangleShape& leftPaddle, sf::CircleShape& ball, float& ballAngle) {
  if (ball.getPosition().x - ballRadius <
          leftPaddle.getPosition().x + paddleSize.x / 2 &&
      ball.getPosition().x - ballRadius > leftPaddle.getPosition().x &&
      ball.getPosition().y + ballRadius >=
          leftPaddle.getPosition().y - paddleSize.y / 2 &&
      ball.getPosition().y - ballRadius <=
          leftPaddle.getPosition().y + paddleSize.y / 2) {
    if (ball.getPosition().y > leftPaddle.getPosition().y)
      ballAngle = pi - ballAngle + (std::rand() % 20) * pi / 180;
    else
      ballAngle = pi - ballAngle - (std::rand() % 20) * pi / 180;

    ball.setPosition(leftPaddle.getPosition().x + ballRadius +
                         paddleSize.x / 2 + 0.1f,
                     ball.getPosition().y);
  }

}

void checkRightPaddleCollision(sf::RectangleShape& rightPaddle, sf::CircleShape& ball, float& ballAngle) {
  if (ball.getPosition().x + ballRadius >
          rightPaddle.getPosition().x - paddleSize.x / 2 &&
      ball.getPosition().x + ballRadius < rightPaddle.getPosition().x &&
      ball.getPosition().y + ballRadius >=
          rightPaddle.getPosition().y - paddleSize.y / 2 &&
      ball.getPosition().y - ballRadius <=
          rightPaddle.getPosition().y + paddleSize.y / 2) {
    if (ball.getPosition().y > rightPaddle.getPosition().y)
      ballAngle = pi - ballAngle + (std::rand() % 20) * pi / 180;
    else
      ballAngle = pi - ballAngle - (std::rand() % 20) * pi / 180;

    ball.setPosition(rightPaddle.getPosition().x - ballRadius -
                         paddleSize.x / 2 - 0.1f,
                     ball.getPosition().y);
  }
}



///////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////PLAYER HANDLING ////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////

void do_enemy_move(string direction,sf::RectangleShape& leftPaddle,sf::RectangleShape& rightPaddle,sf::RectangleShape& UpperPaddle,sf::RectangleShape& DownPaddle, int x , int y)
  {

    cout << direction << endl;


    if (direction.compare("left") == 0)
      {
        leftPaddle.setPosition(x, y);
      }
    if (direction.compare("right") == 0)
      {
        rightPaddle.setPosition(x, y);
      }
    if (direction.compare("up") == 0)
      {
        UpperPaddle.setPosition(x, y);
      }
    if (direction.compare("down") == 0)
      {
        DownPaddle.setPosition(x, y);
      }
  }




///////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////

int main (int argc, char** argv)
{

//////////////////////////////////////////////////


////////INICIO FRAGMENTO

 if (argc != 2) { ///arg se recibe por consola
    cerr << "Wrong execution!!" << endl << "./client ID" << endl;
    return 1;
  }

  zctx_t* context = zctx_new();
  void* client = zsocket_new(context, ZMQ_DEALER);
  //zsocket_connect(client, "tcp://192.168.8.214:5555");
  zsocket_connect(client, "tcp://localhost:5555");

  // This is very strange, with this initialization the application wont work.
  //
  // zsock_t* client = zsock_new(ZMQ_DEALER);
  // int result = zsock_connect(client, "tcp://localhost:5555");
  // cout << "Connection result " << result << endl;

  string myName(argv[1]);
  zmq_pollitem_t items[] = {{client, 0, ZMQ_POLLIN, 0}};

  sendMsg(client, {"join", myName});

//////////////////////////////////////////////
  //////////inicio parte grafica
/////////////////////////////////////////////
    // Define some constants

  // Create the window of the application
  sf::RenderWindow window(sf::VideoMode(gameWidth, gameHeight, 32), "NET Pong",
                          sf::Style::Titlebar | sf::Style::Close);
  window.setVerticalSyncEnabled(true);

  // Create the left paddle
  sf::RectangleShape leftPaddle = createLeftPaddle();

  // Create the right paddle
  sf::RectangleShape rightPaddle = createRightPaddle();

  //create upper paggle
  sf::RectangleShape upperPaddle = createUpperPaddle();

  sf::RectangleShape downPaddle = createDownPaddle();


  // Create the ball
  sf::CircleShape ball = createBall();

  sf::Font font;
  if (!font.loadFromFile("sansation.ttf"))
    return EXIT_FAILURE;



  // Define the paddles properties
  sf::Clock AITimer;
  const sf::Time AITime = sf::seconds(0.1f);
  float rightPaddleSpeed = 0.f;
  const float ballSpeed = 0.f;
  float ballAngle = 0.f; // to be changed later
	
  sf::Clock clock;
  bool isPlaying = false;
  bool scored=false;

    // Initialize the pause message
  sf::Text pauseMessage;
  pauseMessage.setFont(font);
  pauseMessage.setCharacterSize(40);
  pauseMessage.setPosition(170.f, 150.f);
  pauseMessage.setColor(sf::Color::White);
  pauseMessage.setString("Bienvenido a net pong!\nEsperando al oponente");
  window.draw(pauseMessage);
  window.display();


  // Wait for the game to start

  //////RECIEVING LIST OF PLAYERS
  zmsg_t* player_string = zmsg_recv(client);
  //zmsg_print(player_string);
  zframe_t* player = zmsg_pop(player_string);

  // (re)start the game
  isPlaying = true;
  clock.restart();
  startScene(leftPaddle, rightPaddle,upperPaddle,downPaddle, ball, ballAngle);


///////FIN FRAGMENTO


///////////////////////////////////////////////////////  



   // zsock_t *client = zsock_new_dealer("tcp://localhost:5555");
    
   // zstr_send(client,"registro");
   // sleep(5);
   // zmsg_t* resp = zmsg_recv(client);
   // zmsg_print(resp);
    
   //std::srand(static_cast<unsigned int>(std::time(NULL)));


   ///ERORS DE CODIGO PASADO zmq_pollitem_t items[] = {{client, 0, ZMQ_POLLIN, 0}};
  /*
   * 
   * INICIO WHILE DEL JUEGO
   * */


  while (window.isOpen()) {

    int st = zmq_poll(items, 1, 10 * ZMQ_POLL_MSEC);
    if (st == -1) {
      // Handles termination by ^C
      break;
    }
    

    // Handle events
    sf::Event event;
    while (window.pollEvent(event)) {
      if (checkTermination(event)) {
        window.close();
        break;
      }

      // Space key pressed: play
      if ((event.type == sf::Event::KeyPressed) &&
          (event.key.code == sf::Keyboard::Space)) {
        if (!isPlaying) {
          // (re)start the game
          isPlaying = true;
          clock.restart();
          startScene(leftPaddle, rightPaddle,upperPaddle,downPaddle, ball, ballAngle);
      
        }
      }
    }



    if (isPlaying) 
    {
      float deltaTime = clock.restart().asSeconds();
      //void movePlayer1Paddle(sf::RectangleShape& paddle, float deltaTime, char& buffer, void requester, char& Resulta)
      
      
if(zframe_streq(player,"jugador1"))
{
			
      movePlayer1Paddle(leftPaddle,deltaTime,client,"left"); //LLAMADO A AL MOVIMIENTO DE LA PALETA IZQUIERDA
     

            // Move the ball
      moveBall(ball, ballSpeed, ballAngle, deltaTime);
      int ballx = ball.getPosition().x;
      int bally = ball.getPosition().y;
      string posBallx= intToS(ballx);
      string posBally=intToS(bally);
      //sendMsg(client, {"ballpos", posBallx,posBally,"jugando"});

//////COMPUTER MOVE COMING FROM PLAYER ACROSS SERVER
    if (items[0].revents & ZMQ_POLLIN) {
      // This is executed if there is data in the client socket that corresponds
      // to items[0]
      cout << "PLAYER1: " << endl;
      zmsg_t* msg = zmsg_recv(client);
      
      //zmsg_print(msg);

      char* quemado = zmsg_popstr(msg);
      char* datos = zmsg_popstr(msg);
      char* datos2 = zmsg_popstr(msg);
      char* datos3 = zmsg_popstr(msg);
      //int rightPaddlePosy= atoi(datos2);
      //int rightPaddlePosx = atoi(datos3);


      cout << "quemado "<< quemado << endl;
      cout << "datos1 "<< datos << endl;
      cout << "datos2 "<< datos2 << endl;
      cout << "datos3 "<< datos3 << endl;
      /*cout << "datos2 "<< atoi(datos2) << endl;
      cout << "datos3 "<< atoi(datos3) << endl;
      int x =  atoi(datos2);
      int y =  atoi(datos3);
      do_enemy_move(datos, leftPaddle, rightPaddle, upperPaddle, downPaddle, x, y);*/

      zmsg_destroy(&msg);

    }
}
//JUGADOR 2
if(zframe_streq(player,"jugador2"))
  {
        movePlayer1Paddle(rightPaddle, deltaTime, client,"right"); //LLAMADO A AL MOVIMIENTO DE LA PALETA IZQUIERDA
      

      /*BOLA MOVIDA POR POSICION.

      zmsg_t* msg = zmsg_recv(client);
      char* posx = zmsg_popstr(msg);
      char* posy = zmsg_popstr(msg);
      //char* estado = zmsg_popstr(msg);
      zframe_t* estado = zmsg_pop(msg);
      //if(zframe_streq(estado,"jugando")){
        int posxint = atoi(posx);
        int posyint = atoi(posy);
        ball.setPosition(posxint,posyint);
      
      */

      /*}else{
        scored=true;
        }
      zmsg_destroy(&msg); */
      

    if (items[0].revents & ZMQ_POLLIN) {
      // This is executed if there is data in the client socket that corresponds
      // to items[0]
      //cout << "Incoming message: ";
      cout << "PLAYER2: " << endl;
      zmsg_t* msg = zmsg_recv(client);
      //zmsg_print(msg);

      char* quemado = zmsg_popstr(msg);
      char* datos = zmsg_popstr(msg);
      char* datos2 = zmsg_popstr(msg);
      char* datos3 = zmsg_popstr(msg);
      //int rightPaddlePosy= atoi(datos2);
      //int rightPaddlePosx = atoi(datos3);


      cout << "quemado "<< quemado << endl;
      cout << "datos1 "<< datos << endl;
      cout << "datos2 "<< datos2 << endl;
      cout << "datos3 "<< datos3 << endl;
      /*cout << "datos2 "<< atoi(datos2) << endl;
      cout << "datos3 "<< atoi(datos3) << endl;
      int x =  atoi(datos2);
      int y =  atoi(datos3);;

      do_enemy_move(datos, leftPaddle, rightPaddle, upperPaddle, downPaddle, x, y);*/

      zmsg_destroy(&msg);
    }
    
  
}
if(zframe_streq(player,"jugador3"))
  {
        movePlayer1PaddleHorizontal(upperPaddle, deltaTime, client,"up"); //LLAMADO A AL MOVIMIENTO DE LA PALETA IZQUIERDA

      /*BOLA MOVIDA POR POSICION.

      zmsg_t* msg = zmsg_recv(client);
      char* posx = zmsg_popstr(msg);
      char* posy = zmsg_popstr(msg);
      //char* estado = zmsg_popstr(msg);
      zframe_t* estado = zmsg_pop(msg);
      //if(zframe_streq(estado,"jugando")){
        int posxint = atoi(posx);
        int posyint = atoi(posy);
        ball.setPosition(posxint,posyint);
      
      */

      /*}else{
        scored=true;
        }
      zmsg_destroy(&msg); */
      

    if (items[0].revents & ZMQ_POLLIN) {
      // This is executed if there is data in the client socket that corresponds
      // to items[0]
      //cout << "Incoming message: ";
      cout << "PLAYER3: " << endl;
      zmsg_t* msg = zmsg_recv(client);
      //zmsg_print(msg);

      char* quemado = zmsg_popstr(msg);
      char* datos = zmsg_popstr(msg);
      char* datos2 = zmsg_popstr(msg);
      char* datos3 = zmsg_popstr(msg);
      //int rightPaddlePosy= atoi(datos2);
      //int rightPaddlePosx = atoi(datos3);


      cout << "quemado "<< quemado << endl;
      cout << "datos1 "<< datos << endl;
      cout << "datos2 "<< datos2 << endl;
      cout << "datos3 "<< datos3 << endl;
      /*cout << "datos2 "<< atoi(datos2) << endl;
      cout << "datos3 "<< atoi(datos3) << endl;
      int x =  atoi(datos2);
      int y =  atoi(datos3);;

      do_enemy_move(datos, leftPaddle, rightPaddle, upperPaddle, downPaddle, x, y);*/
      zmsg_destroy(&msg);
    }
    
  
}
if(zframe_streq(player,"jugador4"))
  {
     movePlayer1PaddleHorizontal(downPaddle, deltaTime, client,"down"); //LLAMADO A AL MOVIMIENTO DE LA PALETA IZQUIERDA

      /*BOLA MOVIDA POR POSICION.

      zmsg_t* msg = zmsg_recv(client);
      char* posx = zmsg_popstr(msg);
      char* posy = zmsg_popstr(msg);
      //char* estado = zmsg_popstr(msg);
      zframe_t* estado = zmsg_pop(msg);
      //if(zframe_streq(estado,"jugando")){
        int posxint = atoi(posx);
        int posyint = atoi(posy);
        ball.setPosition(posxint,posyint);
      
      */

      /*}else{
        scored=true;
        }
      zmsg_destroy(&msg); */
      

    if (items[0].revents & ZMQ_POLLIN) {
      // This is executed if there is data in the client socket that corresponds
      // to items[0]
      //cout << "Incoming message: ";
      cout << "PLAYER4: " << endl;
      zmsg_t* msg = zmsg_recv(client);
      //zmsg_print(msg);

      char* quemado = zmsg_popstr(msg);
      char* datos = zmsg_popstr(msg);
      char* datos2 = zmsg_popstr(msg);
      char* datos3 = zmsg_popstr(msg);
      //int rightPaddlePosy= atoi(datos2);
      //int rightPaddlePosx = atoi(datos3);


      cout << "quemado "<< quemado << endl;
      cout << "datos1 "<< datos << endl;
      cout << "datos2 "<< datos2 << endl;
      cout << "datos3 "<< datos3 << endl;

      /*cout << "datos2 "<< atoi(datos2) << endl;
      cout << "datos3 "<< atoi(datos3) << endl;
      int x =  atoi(datos2);
      int y =  atoi(datos3);;

      do_enemy_move(datos, leftPaddle, rightPaddle, upperPaddle, downPaddle, x, y);*/

      zmsg_destroy(&msg);
    }
    
  }

      // Move the ball
      //moveBall(ball, ballSpeed, ballAngle, deltaTime);
      // Check if there has been an annotation
     //scored = checkScore(ball);
      if(scored) {
        isPlaying = false;
        //sendMsg(client, {"ballpos","0","0","perdio"});
        //cerr<<"IM HERE MOTHERFUCKER";
        pauseMessage.setString(
            "Game over!!!\nPress space to restart or\nescape to exit");
		//zmsg_t* salida = zmsg_recv(client);
        ///////ENVIAR DESDE AQUI	
            
      }

      // Check the collisions with the lateral borders
      //checkLateralCollision(ball, ballAngle);
      // Check the collisions between the ball and the paddles
      // Left Paddle
      checkLeftPaddleCollision(leftPaddle, ball,  ballAngle);
      // Right Paddle
      checkRightPaddleCollision(rightPaddle, ball, ballAngle);

      checkUpperPaddleCollision(upperPaddle, ball, ballAngle);

      checkLowerPaddleCollision(downPaddle, ball, ballAngle);
    }

    // Clear the window
    window.clear(sf::Color(50, 200, 50));

    if (isPlaying) {
      // Draw the paddles and the ball
      window.draw(leftPaddle);
      window.draw(rightPaddle);
      window.draw(upperPaddle);
      window.draw(downPaddle);
      window.draw(ball);
    } else {
      // Draw the pause message
      window.draw(pauseMessage);
    }

    // Display things on screen
    window.display();
   


  } //fin del while de juego
	zmsg_destroy(&player_string);
	zframe_destroy(&player);
	//zsock_destroy(&client);
  zctx_destroy(&context);
  return EXIT_SUCCESS;
}
