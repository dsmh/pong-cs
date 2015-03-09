
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
const float pi = 3.14159f;
const float paddleSpeed = 400.f;
const int gameWidth = 800;
const int gameHeight = 600;
const float ballRadius = 10.f;

 
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

void startScene(sf::RectangleShape& leftPaddle, sf::RectangleShape& rightPaddle,
                sf::CircleShape& ball, float& ballAngle) {
  // Reset the position of the paddles and ball
  leftPaddle.setPosition(10 + paddleSize.x / 2, gameHeight / 2);
  rightPaddle.setPosition(gameWidth - 10 - paddleSize.x / 2, gameHeight / 2);
  ball.setPosition(gameWidth / 2, gameHeight / 2);

  // Reset the ball angle
  do {
    // Make sure the ball initial angle is not too much vertical
    ballAngle = (5000 % 360) * 2 * pi / 360;
  } while (std::abs(std::cos(ballAngle)) < 0.7f);
}




///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
void sendData(int y,zsock_t *client)
{
    
    stringstream ss;
    ss << y;
    zmsg_t * m = zmsg_new();
    zmsg_addstr(m,"cliente");
    zmsg_addstr(m,ss.str().c_str());
    //cout << y << endl;
    zmsg_send(&m,client);
    //zmsg_t* resp = zmsg_recv(client);
  
  //zmsg_destroy(&resp);
  //zsock_destroy (&client);
}

/*
 * MOVIMIENTO DE LA PALETA
 * CONTINUAR DESDE AQUI , zsock_t *socket
 * */

void movePlayer1Paddle(sf::RectangleShape& paddle, float deltaTime, void* client) {


  // Move the player's paddle
  if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up) &&
      (paddle.getPosition().y - paddleSize.y / 2 > 5.f)) {


            ///sendData(paddle.getPosition().y,socket);
            sendMsg(client,{"move", intToS(paddle.getPosition().y)});
            //cout << paddle.getPosition().y<<endl;

           paddle.move(0.f, -paddleSpeed * deltaTime);
  }
  if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down) &&
      (paddle.getPosition().y + paddleSize.y / 2 < gameHeight - 5.f)) {
          
            ///sendData(paddle.getPosition().y,socket);
            sendMsg(client,{"move", intToS(paddle.getPosition().y)});
            //cout << paddle.getPosition().y<<endl;

    paddle.move(0.f, paddleSpeed * deltaTime);
  }
}

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
  zmsg_t* player_string = zmsg_recv(client);
  zmsg_print(player_string);
  zframe_t* player = zmsg_pop(player_string);

  // (re)start the game
  isPlaying = true;
  clock.restart();
  startScene(leftPaddle, rightPaddle, ball, ballAngle);


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
          startScene(leftPaddle, rightPaddle, ball, ballAngle);
      
        }
      }
    }



    if (isPlaying) 
    {
      float deltaTime = clock.restart().asSeconds();
      //void movePlayer1Paddle(sf::RectangleShape& paddle, float deltaTime, char& buffer, void requester, char& Resulta)
      
      
if(zframe_streq(player,"jugador1"))
{
			
      
      movePlayer1Paddle(leftPaddle, deltaTime, client); //LLAMADO A AL MOVIMIENTO DE LA PALETA IZQUIERDA


//////COMPUTER MOVE COMING FROM PLAYER ACROSS SERVER

    if (items[0].revents & ZMQ_POLLIN) {
      // This is executed if there is data in the client socket that corresponds
      // to items[0]
      cout << "Incoming message: ";
      zmsg_t* msg = zmsg_recv(client);
      char* quemado = zmsg_popstr(msg);
      char* datos = zmsg_popstr(msg);
      int rightPaddlePos = atoi(datos);
      cout << rightPaddlePos << endl;
      rightPaddle.setPosition(786, rightPaddlePos);
      //rightPaddle.setOrigin(rightPaddlePos / 2.f);
      //zmsg_print(msg);
      zmsg_destroy(&msg);
    }
}
else
	{
		    movePlayer1Paddle(rightPaddle, deltaTime, client); //LLAMADO A AL MOVIMIENTO DE LA PALETA IZQUIERDA


//////COMPUTER MOVE COMING FROM PLAYER ACROSS SERVER

    if (items[0].revents & ZMQ_POLLIN) {
      // This is executed if there is data in the client socket that corresponds
      // to items[0]
      cout << "Incoming message: ";
      zmsg_t* msg = zmsg_recv(client);
      char* quemado = zmsg_popstr(msg);
      char* datos = zmsg_popstr(msg);
      int leftPaddlePos = atoi(datos);
      cout << leftPaddlePos << endl;
      leftPaddle.setPosition(0, leftPaddlePos);
      //rightPaddle.setOrigin(rightPaddlePos / 2.f);
      //zmsg_print(msg);
      zmsg_destroy(&msg);
    }
		
	
	}
    //sendMsg(client,{"move", myName});   ///Puesto en la parte de deteccion de teclas



/*
      // Move the computer's paddle
      if (((rightPaddleSpeed < 0.f) &&
           (rightPaddle.getPosition().y - paddleSize.y / 2 > 5.f)) ||
          ((rightPaddleSpeed > 0.f) &&
           (rightPaddle.getPosition().y + paddleSize.y / 2 <
            gameHeight - 5.f))) {
        rightPaddle.move(0.f, rightPaddleSpeed * deltaTime);
      }

      // Update the computer's paddle direction according to the ball position
      if (AITimer.getElapsedTime() > AITime) {
        AITimer.restart();
        if (ball.getPosition().y + ballRadius >
            rightPaddle.getPosition().y + paddleSize.y / 2)
          rightPaddleSpeed = paddleSpeed;
        else if (ball.getPosition().y - ballRadius <  
                 rightPaddle.getPosition().y - paddleSize.y / 2)
          rightPaddleSpeed = -paddleSpeed;
        else
          rightPaddleSpeed = 0.f;
      }


*/

      // Move the ball
      moveBall(ball, ballSpeed, ballAngle, deltaTime);

      // Check if there has been an annotation
      bool scored = checkScore(ball);
      if(scored) {
        isPlaying = false;
        pauseMessage.setString(
            "Game over!!!\nPress space to restart or\nescape to exit");
      }

      // Check the collisions with the lateral borders
      checkLateralCollision(ball, ballAngle);
      // Check the collisions between the ball and the paddles
      // Left Paddle
      checkLeftPaddleCollision(leftPaddle, ball,  ballAngle);
      // Right Paddle
      checkRightPaddleCollision(rightPaddle, ball, ballAngle);
    }

    // Clear the window
    window.clear(sf::Color(50, 200, 50));

    if (isPlaying) {
      // Draw the paddles and the ball
      window.draw(leftPaddle);
      window.draw(rightPaddle);
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
  zctx_destroy(&context);
  return EXIT_SUCCESS;
}
