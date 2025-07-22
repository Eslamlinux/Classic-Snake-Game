#include <iostream>
// #include <conio.h>
// #include <stdio.h>
#include <termios.h> // لمكتبة termios
#include <unistd.h>  // لـ read و STDIN_FILENO
#include <fcntl.h>   // لـ fcntl

using namespace std;

// دالة لتغيير وضع الطرفية (terminal) عشان نقرا حرف واحد بدون Enter
struct termios old_tio, new_tio;

void init_termios(int echo) {
    tcgetattr(STDIN_FILENO, &old_tio); // حفظ إعدادات الطرفية القديمة
    new_tio = old_tio;
    new_tio.c_lflag &= ~(ICANON | ECHO); // تعطيل وضع الـ canonical والإظهار
    tcsetattr(STDIN_FILENO, TCSANOW, &new_tio); // تطبيق الإعدادات الجديدة
}

void reset_termios(void) {
    tcsetattr(STDIN_FILENO, TCSANOW, &old_tio); // استعادة الإعدادات القديمة
}

// دالة شبيهة بـ _kbhit()
int _kbhit() {
    struct timeval tv = {0L, 0L};
    fd_set fds;
    FD_ZERO(&fds);
    FD_SET(STDIN_FILENO, &fds);
    return select(STDIN_FILENO + 1, &fds, NULL, NULL, &tv);
}

// دالة بديلة بـ _getche()
char _getch_linux() {
    char ch;
    read(STDIN_FILENO, &ch, 1);
    return ch;
}


bool gameOver ;
const int width = 25;
const int height = 25;
int x, y ,fruitA , fruitB , score;
enum eDirection { STOP = 0, LEFT, RIGHT, UP ,DOWN };
eDirection dir;

int tailX[100];
int taily[100];
int inTail = 0;

int speedcount = 0;

struct Colors{
   string Red = "\e[31m";
   string Green = "\e[32m";
   string Yellow = "\e[33m";
   string Blue = "\e[34m";
   string Purple = "\e[35m";
   string Cyan = "\e[36m";
   string White = "\e[37m";
   string Reset = "\e[0m";
   };

    Colors gamecolor;


void setup(){
    gameOver = false;
    dir = STOP;
    x = width / 2;
    y = height / 2;
    fruitA = rand() % width;
    fruitB = rand() % height;
    score = 0;
}


void graf(){
    system("clear"); // for linux
    // system("cls"); for windows
    // اعلي الشاشة
    for(int i = 0 ; i < width + 2 ;i++){


        cout << gamecolor.Green << "-" << gamecolor.Reset;
    }
        cout << endl;

    // جانبي الشاشة
    for(int i = 0 ; i < height ; i++){
        cout << gamecolor.Green << '|' << gamecolor.Reset;
        for(int j = 0 ; j < width ; j++){
            if(i == y && j == x){
                cout << gamecolor.Red << '@' << gamecolor.Reset;
            }
            else if(i == fruitB && j == fruitA){
                cout << gamecolor.Yellow << 'O' << gamecolor.Reset;
            }
            else{
                bool print = false;
                for(int k = 0 ; k < inTail ; k++){
                    if(tailX[k] == j && taily[k] == i){
                        cout << gamecolor.Yellow << 'o' << gamecolor.Reset;
                        print = true;
                        break;
                    }
                }
                if(!print){
                    cout << " ";
                }
            }
        }
        cout << gamecolor.Green << '|' << gamecolor.Reset;
        cout << endl;
    }
    // اسفل الشاشة
     for(int i = 0 ; i < width +2 ;i++){
        cout << gamecolor.Green << '-' << gamecolor.Reset ;
    }
    cout << endl;
    cout << gamecolor.Purple << "your score is: " << gamecolor.Cyan << score << gamecolor.Reset << endl;
    cout << gamecolor.Purple << "Snake Game Made By:" << gamecolor.Cyan << " Eslam Linux " << endl;
    cout << gamecolor.Purple << "Speed Level: " << gamecolor.Cyan << speedcount << endl;



}

void input(){
    if(_kbhit()){
        switch(_getch_linux()){
            case 'a': dir = LEFT;
            break;
            case 'd': dir = RIGHT;
            break;
            case 'w': dir = UP;
            break;
            case 's': dir = DOWN;
            break;
            case 'x': gameOver = true;
            break;

        }
    }

}

void logic(){
    int prevXtail = tailX[0];
    int prevYtail = taily[0];
    int prev2x ,prev2y;
    tailX[0] = x;
    taily[0] = y;
    for(int i = 1 ; i < inTail ; i++){
        prev2x = tailX[i];
        prev2y = taily[i];
        tailX[i] = prevXtail;
        taily[i] = prevYtail;
        prevXtail = prev2x;
        prevYtail = prev2y;
    }
    // تحريك رأس الثعبان حسب الاتجاه
    switch(dir){
        case LEFT: x--;
        break;
        case RIGHT: x++;
        break;
        case UP: y--;
        break;
        case DOWN: y++;
        break;
        default:
        break;
    }
    // التحقق إذا خرج الثعبان من حدود الشاشة
    if(x >= width || x < 0 || y >= height || y < 0){
        gameOver = true;
    }
    // التحقق إذا اصطدم الثعبان بنفسه
    for(int i = 0 ; i < inTail ;i++){
        if(tailX[i] == x && taily[i] == y){
            gameOver = true;
        }
    }
    // أكل الفاكهة
    if(x == fruitA && y == fruitB){
        score++;
        fruitA = rand() % width;
        fruitB = rand() % height;
        inTail++;
    }

}

void speed(){
   // Sleep(10); in windows
         if(score < 3){
            usleep(400000);
            if(score == 2){
            speedcount = 1;
            }
        }
          if(score >= 3 && score <6){
            usleep(360000);
            if(score == 5){
            speedcount = 2;
            }
        }
              if(score >= 6 && score <8){
            usleep(320000);
             if(score == 7){
            speedcount = 3;
            }
        }
        if(score > 7 && score < 14){
            usleep(310000);
             if(score == 13){
            speedcount = 4;
            }
        }
         if(score >= 14 && score < 18){
            usleep(180000);
             if(score == 17){
            speedcount = 5;
            }
        }
            if(score >= 18 && score < 20){
            usleep(170000);
             if(score == 19){
            speedcount = 6;
            }
        }
          if(score >= 20 && score < 23){
            usleep(160000);
             if(score == 22){
            speedcount = 7;
            }
        }
              if(score >= 23 && score < 25){
            usleep(150000);
             if(score == 24){
            speedcount = 7;
            }
        }
              if(score >= 25 && score < 28){
            usleep(140000);
             if(score == 27){
            speedcount = 7;
            }
        }
              if(score >= 28){
            usleep(130000);
             if(score == 27){
            speedcount = 7;
            }
        }
}


int main (){
    init_termios(0); // تهيئة الطرفية قبل بداية اللعبة

    setup();
    while(!gameOver){
        graf();
        input();
        logic();
        speed();

    }

    if(gameOver){
    cout << gamecolor.Red << "\n GAME OVER \n" << gamecolor.Reset;

}

        reset_termios(); // استعادة إعدادات الطرفية عند نهاية اللعبة
    return 0;
}
