//This program is a battleship game
#include <iostream>
#include <cstdlib>
#include <cstring>
#include <string>
using namespace std;

//Template to verify user input data type
template <class T>
bool t_Verify(T input){
  if(!cin){
    cin.clear();
    cin.ignore(100,'\n');
    return false;
  }
  else{
    return true;
  }
}

//Game functions just like a battleship game, the game instructions will be included within the program
//The grid uses numbers to represent different objects
//0 means that there is nothing on the grid, it is the default of the grid
//1 to SHIPS represents the different ships
//99 means that someone has fired on this grid but nothing was hit
//100 means that someone fired on this grid and there was a ship on it
class Game{
private:
//determines the grid size
  static const int G_SIZE=10;
  int grid[G_SIZE][G_SIZE];
public:
  int o_GSize(){return G_SIZE;};
  int o_Grid(int y,int x){return grid[y][x];};
  void i_Grid(int y,int x,int k){grid[y][x]=k;};
  Game(){
    for(int y=0;y<G_SIZE;y++){
      for(int x=0;x<G_SIZE;x++){
        grid[y][x]=0;
      }
    }
  }
//function to reset grid
  void reset_G();
};

//function to reset grid
void Game::reset_G(){
  for(int y=0;y<G_SIZE;y++){
    for(int x=0;x<G_SIZE;x++){
      grid[y][x]=0;
    }
  }
}

class Ships: public Game{
private:
//determines the total number of ships
  static const int SHIPS=5;
//record which ships are standing,true means that the ships still remains on the grid and false means that it has been sunk
  bool remain[SHIPS];
  int size[SHIPS];
//stores the starting points and orientation of the ships
  int start_C[SHIPS][3];
public:
//determines the size of the ships
  Ships(){
    size[0]=5;
    size[1]=4;
    size[2]=3;
    size[3]=3;
    size[4]=2;
    for(int i=0;i<SHIPS;i++){
      remain[i]=true;
    }
  }
//returns the number of ships
  int o_Num(){return SHIPS;};
//returns the size of the ship
  int o_Size(int i){return size[i];};
//stores the origin coordinates and orientation of the ships
  void i_Start(int iD,int y,int x,int vert){start_C[iD][0]=y;start_C[iD][1]=x;start_C[iD][2]=vert;};
//returns the origin y coordinates of the ships
  int o_Y(int iD){return start_C[iD][0];};
//returns the origin y coordinates of the ships
  int o_X(int iD){return start_C[iD][1];};
//returns the orientation of the ships
  int o_Vert(int iD){return start_C[iD][2];};
//stores if the ship still floats
  void i_Remain(int iD,bool flies){remain[iD]=flies;}
//returns if the ships still floats
  bool o_Remain(int iD){return remain[iD];};
};

//function prototypes
//the starting menu function, allows the user to choose the different modes
int s_Menu();
//function randomly places the ships
void r_Place(Ships &ship);
//this function 'builds' the ship
bool build(Ships &ship,int iD,int y,int x,int vert);
//function to display grid
void g_Display(Ships &ship);
//function to let the user change ship position
void steer(Ships &ship);
//function to remove ships from the grid
void g_Void(Ships &ship,int iD);
//function to show user their game grid instead of the ship building grid as it would not look nice
void game_Grid(Ships &ship);
//function to show user their opponent's game grid instead of the ship building grid as it would not look nice
void fight_Grid(Ships &ship);
//function to convert letters to numbers
int let_To_Num(char letter);
//function to convert numbers to letters needed for inputs
char num_To_Let(int number);
//function for user to fire
void cannon(Ships &ship);
//checks if all ships has been sunk
bool check_All(Ships &ship);
//checks if a certain ship is sunk
bool check_Dis(Ships &ship,int k);
//function for computer to fire
void mind_Control(Ships &ship);
void instructions();

int main(){
  srand(time(NULL));
//Using pointers for debugging purposes
//Stores the user ship data
  Ships *u_Ship=new Ships;
//stores the computer or player 2 ship data
  Ships *c_Ship=new Ships;
//determines if the game is over, the program is over
  bool g_Over=false,g_End=false;
//stores the value of user input
  int input;
  cout<<"Welcome to Battleship the game!\n";
  do{
    input=s_Menu();
    if(input==2){
      g_End=true;
    }
    else if(input==1){
      instructions();
//places the ships randomly
      cout<<"You are player 1.\n";
      r_Place(*u_Ship);
      r_Place(*c_Ship);
      cout<<"The following is your grid. Each number represent a ship part that belongs to a different ship. You are free to make any changes to this randomly generated grid.\n";
      g_Display(*u_Ship);
      steer(*u_Ship);
      do{
        cout<<"\nPlayer 1's turn!\n";
        fight_Grid(*c_Ship);
        cannon(*c_Ship);
        if(check_All(*c_Ship)==false){
          cout<<"Player 1 won! All of the player 2's ships have been sunk!\n";
          g_Over=true;
        }
        else if(check_All(*u_Ship)==false){
          cout<<"Player 2 won! All of player 1's ships have been sunk!\n";
          g_Over=true;
        }
        if(g_Over==false){
          cout<<"\nPlayer 2's turn!\n";
          mind_Control(*u_Ship);
          game_Grid(*u_Ship);
        }
      }while(g_Over==false);
      delete u_Ship;
      delete c_Ship;
    }
  }while(g_End==false);
  delete u_Ship;
  delete c_Ship;
  return 0;
}

//the starting menu function, allows the user to choose the different modes
int s_Menu(){
//stores the user input
  int input;
//stores the value if the input is allowed to return to the main function
  bool good=false;
  do{
    cout<<"MENU:\n"
    <<"===================================================\n"
    <<"1. Single player\n"
    <<"2. Quit\n"
    <<"Please enter the numbers representing the above options.\n";
    cin>>input;
    good=t_Verify(input);
    if(good==true){
      if((input<1)||(input>2)){
        cout<<"Invalid range! Please enter the numbers representing the menu options.\n";
        good=false;
      }
    }
    else{
      cout<<"Invalid input! Please enter only numbers!\n";
    }
  }while(good==false);
  return input;
}

/*The ships will be placed randomly originally then the user can manually move the ships*/
//function randomly places the ships
void r_Place(Ships &ship){
//starting y and x coordinates and if the ship is vertical respectively
//if vert is 0 then it is horrizontal and if it is 1 then it is vertical
  int y,x,vert;
//check if the ship has been placed
  bool placed=false;
  for(int i=0;i<ship.o_Num();i++){
    do{
      y=rand()%10;
      x=rand()%10;
//this ensures that the function to build is only called if the initial coordinates is empty
      if(ship.o_Grid(y, x)==0){
        vert=rand()%2;
        placed=build(ship,i,y,x,vert);
      }
    }while(placed==false);
    placed=false;
  }
}

//this function 'builds' the ship
bool build(Ships &ship,int iD,int y,int x,int vert){
  if((y<0)||(x<0)){
    return false;
  }
  int temp_X=x,temp_Y=y;
  if(vert==0){
//this loop ensures that the ship will have no overlaps with any other ships
    for(int i=0;i<ship.o_Size(iD);i++){
//this check if the grid is either empty or is the ship itself
      if((ship.o_Grid(y, temp_X)!=0)&&(ship.o_Grid(y,temp_X)!=(iD+1))){
        return false;
      }
      if(temp_X>=(ship.o_GSize()-1)){
        return false;
      }
      temp_X++;
    }
    temp_X=x;
//this empties the chosen grid to create the illusion of moving the ships
    g_Void(ship, iD);
//this loop creates the ship
    for(int k=0;k<ship.o_Size(iD);k++){
      ship.i_Grid(y, temp_X, (iD+1));
      temp_X++;
    }
  }
  else{
//this loop ensures that the ship will have no overlaps with any other ships
    for(int i=0;i<ship.o_Size(iD);i++){
//this check if the grid is either empty or is the ship itself
      if((ship.o_Grid(temp_Y, x)!=0)&&(ship.o_Grid(temp_Y,x)!=(iD+1))){
        return false;
      }
      if(temp_Y>=(ship.o_GSize()-1)){
        return false;
      }
      temp_Y++;
    }
    temp_Y=y;
//this empties the chosen grid to create the illusion of moving the ships
    g_Void(ship, iD);
//this loop creates the ship
    for(int k=0;k<ship.o_Size(iD);k++){
      ship.i_Grid(temp_Y, x, (iD+1));
      temp_Y++;
    }
  }
//this stores the origin coordinates
  ship.i_Start(iD, y, x,vert);
  return true;
}

//function to display grid
void g_Display(Ships &ship){
  cout<<"\nPlayer 1 Grid:\n"
  <<"===================================================\n"
  <<"  |0|1|2|3|4|5|6|7|8|9|\n"
  <<"  ---------------------\n";
  for(int y=0;y<ship.o_GSize();y++){
    cout<<num_To_Let(y)<<" |";
    for(int x=0;x<ship.o_GSize();x++){
      cout<<ship.o_Grid(y, x)<<"|";
    }
    cout<<endl;
  }
  cout<<"===================================================\n";
}

//function to let the user change ship position
void steer(Ships &ship){
  string s_Input;
  int n_Input,iD,y,x,vert;
  bool confirmed=false,placed=false;
  do{
//asks the user if any changes are desired. If the user is satisfied, the user can enter anything to skip this process
    cout<<"Would you like to change any of the ships' locations? Enter 'Y' to confirm or enter anything else to ignore.\n";
    cin>>s_Input;
    if((s_Input !="Y")&&(s_Input!="y")){
      cout<<"Ships placed!\n";
      confirmed=true;
    }
    else{
//Displays the grid so the user can see clearly of the grid
      g_Display(ship);
      cout<<"Ship list:\n"
      <<"===================================================\n"
      <<"1: Carrier\n"
      <<"2: Battleship\n"
      <<"3: Submarine\n"
      <<"4: Cruiser\n"
      <<"5. Destroyer\n\n"
      <<"Which ship would you like to change? Enter the ship iD from the given list\n";
      cin>>n_Input;
//verifies if the user input has the correct data type
      if(t_Verify(n_Input)==false){
        cout<<"Invalid input! Please enter only the iD number of the ships!\n";
        confirmed=false;
      }
      else{
        confirmed=true;
      }
//If the user enters the correct data type then check if it is one of the options
      if(confirmed==true&&((n_Input<1)||(n_Input>5))){
        cout<<"Invalid input! Please enter the correct iD number of the ships!\n";
        confirmed=false;
      }
//if it checked for both then ask for directions for the ship
      if(confirmed==true){
        iD=n_Input-1;
        do{
          g_Display(ship);
          cout<<"Directions:\n"
          <<"1. Move up\n"
          <<"2. Move down\n"
          <<"3. Move left\n"
          <<"4. Move right\n"
          <<"5. Rotate\n"
          <<"6. End\n"
          <<"Enter the number of the given options to move the ship\n";
          cin>>n_Input;
//checks again to makes ure the user only input the right data
          if(t_Verify(n_Input)==false){
            cout<<"Invalid input! Please enter only numbers!\n";
          }
          else{
            confirmed=true;
          }
//checks to make sure that the user only put in the options given
          if(confirmed==false&&((n_Input<1)||(n_Input>4))){
            cout<<"Invalid input! Please enter the correct number of the options!\n";
          }
          else{
            confirmed=true;
          }
          if(confirmed==true){
            y=ship.o_Y(iD);
            x=ship.o_X(iD);
            vert=ship.o_Vert(iD);
            switch(n_Input){
              case 1:
                y--;
              break;
              case 2:
                y++;
              break;
              case 3:
                x--;
              break;
              case 4:
                x++;
              break;
              case 5:
                if(vert==0){
                  vert=1;
                }
                else{
                  vert=0;
                }
              break;
              case 6:
              placed=true;
              break;
            }
//outputs if the user entered an invalid move
            if(build(ship, iD, y, x, vert)==false){
              cout<<"Invalid move!\n";
            }
          }
        }while(placed==false);
        g_Display(ship);
        confirmed=false;
      }
    }
  }while(confirmed==false);
}

//function to remove ships from the grid
void g_Void(Ships &ship,int iD){
  int y=ship.o_Y(iD),x=ship.o_X(iD);
  if(ship.o_Vert(iD)==0){
//turns the grid to "empty", aka 0
    for(int k=0;k<ship.o_Size(iD);k++){
      ship.i_Grid(y,x, 0);
      x++;
    }
  }
  else{
//turns the grid to "empty", aka 0 but in another orientation
    for(int k=0;k<ship.o_Size(iD);k++){
      ship.i_Grid(y,x, 0);
      y++;
    }
  }
}

//function to show user their game grid instead of the ship building grid as it would not look nice
void game_Grid(Ships &ship){
  cout<<"\nPlayer 1 Grid:\n"
  <<"===================================================\n"
  <<"  |0|1|2|3|4|5|6|7|8|9|\n"
  <<"  ---------------------\n";
  for(int y=0;y<ship.o_GSize();y++){
    cout<<num_To_Let(y)<<" |";
    for(int x=0;x<ship.o_GSize();x++){
      if(ship.o_Grid(y, x)==0){
        cout<<"O|";
      }
      else if(ship.o_Grid(y, x)==99){
        cout<<"B|";
      }
      else if(ship.o_Grid(y, x)==100){
        cout<<"X|";
      }
      else{
        cout<<"S|";
      }
    }
    cout<<endl;
  }
  cout<<"===================================================\n";
}

//function to show user their opponent's game grid instead of the ship building grid as it would not look nice
void fight_Grid(Ships &ship){
  cout<<"\nPlayer 2 Grid:\n"
  <<"===================================================\n"
  <<"  |0|1|2|3|4|5|6|7|8|9|\n"
  <<"  ---------------------\n";
  for(int y=0;y<ship.o_GSize();y++){
    cout<<num_To_Let(y)<<" |";
    for(int x=0;x<ship.o_GSize();x++){
      if(ship.o_Grid(y, x)==99){
        cout<<"B|";
      }
      else if(ship.o_Grid(y, x)==100){
        cout<<"X|";
      }
      else{
        cout<<"O|";
      }
    }
    cout<<endl;
  }
  cout<<"===================================================\n";
}

//function to convert letters to numbers
int let_To_Num(char letter){
  if (letter=='A'){
    return 0;
  }
  else if (letter =='B'){
    return 1;
  }
  else if (letter =='C'){
    return 2;
  }
  else if (letter =='D'){
    return 3;
  }
  else if (letter =='E'){
    return 4;
  }
  else if (letter =='F'){
    return 5;
  }
  else if (letter =='G'){
    return 6;
  }
  else if (letter =='H'){
    return 7;
  }
  else if (letter =='I'){
    return 8;
  }
  else if (letter =='J'){
    return 9;
  }
  return 99;
}

//function to convert numbers to letters needed for inputs
char num_To_Let(int number){
  switch(number){
    case 0: return 'A';
    break;
    case 1: return 'B';
    break;
    case 2: return 'C';
    break;
    case 3: return 'D';
    break;
    case 4: return 'E';
    break;
    case 5: return 'F';
    break;
    case 6: return 'G';
    break;
    case 7: return 'H';
    break;
    case 8: return 'I';
    break;
    case 9: return 'J';
    break;
    default:return 'Z';
    break;
  }
}

//function to let user fire
void cannon(Ships &ship){
  bool fired=false;
  int str_Length,y,x;
  string input;
  do{
    cout<<"Which row would you like to fire?\n";
    cin>>input;
    str_Length=input.length();
    for(int k=0;k<str_Length;k++){
      input[k]=toupper(input[k]);
    }
//if the user enters 2 characters, the user must have entered the row and column together or it would be invalid
    if(str_Length==2){
      if((input[0]=='A')||(input[0]=='B')||(input[0]=='C')||(input[0]=='D')||(input[0]=='E')||(input[0]=='F')||(input[0]=='G')||(input[0]=='H')||(input[0]=='I')||(input[0]=='J')){
        y=let_To_Num(input[0]);
        if((input[1]=='0')||(input[1]=='1')||(input[1]=='2')||(input[1]=='3')||(input[1]=='4')||(input[1]=='5')||(input[1]=='6')||(input[1]=='7')||(input[1]=='8')||(input[1]=='9')){
          x=static_cast<int>(input[1])-48;
          fired=true;
        }
        else{
          cout<<"Invalid input! Please a valid column.\n";
        }
      }
      else if ((input[0]=='0')||(input[0]=='1')||(input[0]=='2')||(input[0]=='3')||(input[0]=='4')||(input[0]=='5')||(input[0]=='6')||(input[0]=='7')||(input[0]=='8')||(input[0]=='9')){
        x=static_cast<int>(input[0])-48;
        if((input[1]=='A')||(input[1]=='B')||(input[1]=='C')||(input[1]=='D')||(input[1]=='E')||(input[1]=='F')||(input[1]=='G')||(input[1]=='H')||(input[1]=='I')||(input[1]=='J')){
          y=let_To_Num(input[1]);
          fired=true;
        }
        else{
          cout<<"Invalid input! Please enter a valid row.\n";
        }
      }
    }
//If the user enters 1 character, the user must have entered the row or the column
    else if(str_Length==1){
      if ((input[0]=='A')||(input[0]=='B')||(input[0]=='C')||(input[0]=='D')||(input[0]=='E')||(input[0]=='F')||(input[0]=='G')||(input[0]=='H')||(input[0]=='I')||(input[0]=='J')){
        y=let_To_Num(input[0]);
        cout<<"Which column would you like to fire on?\n";
        cin>>input;
        if ((input[0]=='0')||(input[0]=='1')||(input[0]=='2')||(input[0]=='3')||(input[0]=='4')||(input[0]=='5')||(input[0]=='6')||(input[0]=='7')||(input[0]=='8')||(input[0]=='9')){
          x=static_cast<int>(input[0])-48;
          fired=true;
        }
        else{
          cout<<"Invalid input! Please enter a valid column.\n";
        }
      }
      else if ((input[0]=='0')||(input[0]=='1')||(input[0]=='2')||(input[0]=='3')||(input[0]=='4')||(input[0]=='5')||(input[0]=='6')||(input[0]=='7')||(input[0]=='8')||(input[0]=='9')){
        x=static_cast<int>(input[0])-48;
        cout<<"Which row would you like to fire on?\n";
        cin>>input;
        if((input[0]=='A')||(input[0]=='B')||(input[0]=='C')||(input[0]=='D')||(input[0]=='E')||(input[0]=='F')||(input[0]=='G')||(input[0]=='H')||(input[0]=='I')||(input[0]=='J')){
          y=let_To_Num(input[0]);
          fired=true;
        }
        else{
          cout<<"Invalid input! Please enter a valid row.\n";
        }
      }
    }
    else{
      cout<<"Invalid input! Please enter only the row and column.\n";
    }
    if(fired==true){
      if((ship.o_Grid(y, x)==99)||(ship.o_Grid(y, x)==100)){
        cout<<"This spot has already been fired on!\n";
        fired=false;
      }
      else if(ship.o_Grid(y, x)!=0){
        cout<<"Ship hit!\n";
        ship.i_Grid(y, x, 100);
        int iD=ship.o_Grid(y, x)-1;
        if(check_Dis(ship,iD)==false){
          cout<<"A ship was sunk!\n";
        }
      }
      else{
        cout<<"Nothing was hit!\n";
        ship.i_Grid(y, x, 99);
      }
    }
  }while(fired==false);
}

//checks if all ships has been sunk
bool check_All(Ships &ship){
  for(int i=0;i<ship.o_Num();i++){
    if(ship.o_Remain(i)==true){
      check_Dis(ship,i);
//if 1 single ship remains, the function will also return a true value
      if(ship.o_Remain(i)==true){
        return true;
      }
    }
  }
  return false;
}

//checks if a ship was sunk
bool check_Dis(Ships &ship,int k){
  int id=0;
  for(int y=0;y<ship.o_GSize();y++){
    for(int x=0;x<ship.o_GSize();x++){
      if(ship.o_Grid(y,x)==(k+1)){
        id++;
      }
    }
  }
  if(id>0){
    ship.i_Remain(k, true);
//the ship is still floating
    return true;
  }
//the ship has been sunk
  return false;
}

//function for computer to fire
void mind_Control(Ships &ship){
//generates random numbers for the ship to fire
//might work on this in the future to improve the accuracy of the AI such as allowing the AI to fire grids near on confirmed ship parts and continue once the orientation was found
  int x=rand()%10;
  int y=rand()%10;
  bool fired=false;
  do{
    if((ship.o_Grid(y, x)==99)||(ship.o_Grid(y, x)==100)){
      x=rand()%10;
      y=rand()%10;
    }
    else if(ship.o_Grid(y, x)==0){
      cout<<"Nothing was hit!\n";
      ship.i_Grid(y, x, 99);
    }
    else{
      cout<<"A ship was sunk!\n";
      ship.i_Grid(y, x, 100);
    }
  }while(fired==true);
}

//function for the instructions
void instructions(){
  cout<<"The object of Battleship is to try and sink all of the other player's before they sink all of your ships. All of the other player's ships are somewhere on his/her board.\n"
  <<"Each player has one turn to choose which grid to strike.\n"
  <<"Once the game begins, you will be able to see the following objects:\n"
  <<"Empty space: O\n"
  <<"Grids that were hit but has no ships on it: B\n"
  <<"Grids that were hit and had a ship on it: X\n"
  <<"Ships: S (On your own grid)\n"
  <<"GOOD LUCK!\n\n";
}
