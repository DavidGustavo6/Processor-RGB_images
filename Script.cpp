#include <iostream>
#include <fstream>
#include "Script.hpp"
#include "PNG.hpp"
#include "XPM2.hpp"
#include <algorithm>

using namespace std;

namespace prog {
    // Use to read color values from a script file.
    istream& operator>>(istream& input, Color& c) {
        int r, g, b;
        input >> r >> g >> b;
        c.red() = r;
        c.green() = g;
        c.blue() = b;
        return input;
    }

    Script::Script(const string& filename) :
        image(nullptr), input(filename) {

    }
    void Script::clear_image_if_any() {
        if (image != nullptr) {
            delete image;
            image = nullptr;
        }
    }
    Script::~Script() {
        clear_image_if_any();
    }

    void Script::run() {
        string command;
        while (input >> command) {
            cout << "Executing command '" << command << "' ..." << endl;
            if (command == "open") {
                open();
                continue;
            }
            if (command == "blank") {
                blank();
                continue;
            }
            // Other commands require an image to be previously loaded.
            if (command == "save") {
                save();
                continue;
            } 
            if(command == "invert"){
                invert();                           //Se o comando corresponder a "invert" será executada uma chamada à função "invert()", que alterará a cor de cada píxel para a sua cor inversa;
                continue;
            }
            if(command == "to_gray_scale"){
                to_gray_scale();                    //Se o comando corresponder a "to_grey_scale" será executada uma chamada à função "to_grey_scale()" que alterará cada porção de cada cor para uma média das 3;
                continue;
            }
            if(command == "replace"){
                replace();                          //Se o comando corresponder a "replace" será executada uma chamada à função "replace()" que alterará o conteúdo de uma cor("r1") para o de uma nova cor("r2");
                continue;
            }
            if(command == "fill"){                  //Se o comando corresponder a "fill" será executada uma chamada à função "fill()" que dentro de um quadrado, fornecido por user input, dentro da imagem, irá alterar as cores dos píxeis para uma cor, também definida por user input;
                fill();
                continue;
            }
            if(command == "h_mirror"){
                h_mirror();                         //Se o comando corresponder a "h_mirror" será executada uma chamada à função "h_mirror()" que espelhará a imagem horizontalmente;
                continue;
            }
            if(command == "v_mirror"){
                v_mirror();                         //Se o comando corresponder a "v_mirror" será executada uma chamada à função "v_mirror()" que espelhará a imagem verticalmente;
                continue;
            }
            if(command == "add"){                   //Se o comando corresponder a "add" será executada uma chamada à função "add()" que fará input a uma nova imagem e copiará para um quadrado da imagem inicial certos píxeis caso estes não sejam iguais a uma cor neutra, também definida por user input;
                add();
                continue;
            }
            if(command == "rotate_left"){
                rotate_left();                      //Se o comando corresponder a "rotate_left" será executada uma chamada à função "rotate_left" que fará uma rotação de 90º para a esquerda;
                continue;
            }
            if(command == "rotate_right"){
                rotate_right();                     //Se o comando corresponder a "rotate_right" será executada uma chamada à função "rotate_right" que fará uam rotação de 90º para a direita;
                continue;
            }
            if(command == "crop"){
                crop();                             //Se o comando corresponder a "crop" será executada uma chamada à função "crop()" que fará um corte na imagem, reduzindo-a a uma pequena secção, definida por user input;
                continue;
            }
            if(command == "median_filter"){
                int ws;
                input >> ws;
                median_filter(ws);                             //Se o comando corresponder a "crop" será executada uma chamada à função "crop()" que fará um corte na imagem, reduzindo-a a uma pequena secção, definida por user input;
                continue;
            }
            if(command == "xpm2_open"){
                string filename;
                input >> filename;
                image = loadFromXPM2(filename);                         //Se o comando corresponder a "crop" será executada uma chamada à função "crop()" que fará um corte na imagem, reduzindo-a a uma pequena secção, definida por user input;
                continue;
            }
            if (command == "xpm2_save") {
                string filename;
                input >> filename;
                saveToXPM2(filename, image);
                continue;
            }
        }
    }
    void Script::open() {
        clear_image_if_any();
        string filename;
        input >> filename;
        image = loadFromPNG(filename);
    }
    void Script::blank() {
        clear_image_if_any();
        int w, h;
        Color fill;
        input >> w >> h >> fill;
        image = new Image(w, h, fill);
    }
    void Script::save() {
        string filename;
        input >> filename;
        saveToPNG(filename, image);
    }
    void Script::invert(){
        for(int x = 0; x<image->width(); x++){                      //Através de estes dois ciclos "for" conseguimos iterar sobre todos os píxeis de uma imagem;
            for(int y = 0; y<image->height(); y++){                     //Deste modo, a cada um destes píxeis iremos inverter a sua cor;
                Color pixel = image->at(x,y);                        //Linha 101- Referência do píxel através da função "at()" definida na classe "Image";
                image->at(x,y).red() = 255- pixel.red();                       //Iversão da porção "Vermelha" da cor;
                image->at(x,y).blue() = 255 - pixel.blue();                    //Iversão da porção "Azul" da cor;
                image->at(x,y).green() = 255 - pixel.green();                  //Iversão da porção "verde" da cor;
            }
        }
    }
    void Script::to_gray_scale(){
        for(int x = 0; x<image->width(); x++){                      //Através de estes dois ciclos "for" conseguimos iterar sobre todos os píxeis de uma imagem;
            for(int y = 0; y<image->height(); y++){      
                Color pixel = image->at(x,y);            //Desta forma, a cada um destes píxeis vamos converter as componentes das 3 cores para o resultado da sua média;                      //Linha 111 - Referência do pixel a que iremos efetuar as alterações descritas previamente;
                //int grey_value = (pixel.red() + pixel.blue() + pixel.green())/3;  //Criação de uma variável do tipo "int" que armazena o valor da "Grey Color";
                image->at(x,y).red() = (pixel.red() + pixel.blue() + pixel.green())/3;                      
                image->at(x,y).blue() = (pixel.red() + pixel.blue() + pixel.green())/3;                   
                image->at(x,y).green() = (pixel.red() + pixel.blue() + pixel.green())/3;     //Alteração da porção da cor "Azul" para a média das 3, sem arredondamento;
            }
        }
    }
    void Script::replace(){
        Color u, v;                                         //Criação de 2 novas variáveis do tipo "Color", para armazenar a cor antiga("r1") e a que será usada para substituir a mesma("r2");
        input >> u >> v;                                    //Através de user input, o conteúdo das variáveis é fornecido;
        for(int x = 0; x<image->width(); x++){                //Através de estes dois ciclos "for" conseguimos iterar sobre todos os píxeis de uma imagem;
            for(int y = 0; y<image->height(); y++){ 
                if(image->at(x,y).red()==u.red() and image->at(x,y).green()==u.green() and image->at(x,y).blue()==u.blue()){
                    image->at(x,y).red() = v.red();
                    image->at(x,y).green() = v.green();
                    image->at(x,y).blue() = v.blue();
                }
            }
        }
    }
    void Script::fill(){
        int x, y, w, h;                                         //Criação de 4 variáveis do tipo "int" para armazenar as dimensões do quadrado, no qual as cores dos píxeis serão alteradas;
        int r, g, b;                                          //Criação de 3 variáveis do tipo "Color" para armazenar as porções de ("Vermelho", "Verde e "Azul"), respetivamente, da cor que pretendemos que os píxeis tomem;
        input >> x >> y >> w >> h >> r >> g >> b;               //Através de user input estas 7 variáveis recebem agora os valores que determinarão as dimensões do quadrado e a cor;
        for(int z = x; z<x+w ; z++){                           //Através de estes dois ciclos "for" conseguimos iterar sobre todos os píxeis que estão dentro dos limites do quadrado;
            for(int l = y; l< y+h; l++){                             //Linha 135 - Referência do píxel que iremos efetuar as alterações descritas;
                image->at(z,l).red() = r;                                //Alteração da porção "Vermelha" da cor do píxel para a porção da cor "Vermelha" da nova cor;
                image->at(z,l).green() = g;                              //Alteração da porção "Verde" da cor do píxel para a porção da cor "Verde" da nova cor;
                image->at(z,l).blue() = b;                               //Alteração da porção "Azul" da cor do píxel para a porção da cor "Azul" da nova cor;
            }
        }
    }
    void Script::h_mirror(){
        for(int y = 0; y< image->height(); y++){                //Uma vez que o objetivo é espelhar horizontamente uma imagem, não necessitamos de iterar sobre todos os píxeis de uma imagem;
            for(int x = 0; x< image->width()/2; x++){           //De facto, ao utilizarmos estes 2 ciclos "for" iremos apenas iterar os pixeis até metade da largura da imagem;                //A variável "pixel3" irá guardar o valor do píxel do seu equivalente ao espelhar a imagem horizontalmente("pixel4");
                Color temporaria = image->at(x,y);
                image->at(x,y) = image->at(image->width()-x-1,y);
                image->at(image->width()-x-1, y) = temporaria;
            }
        }
    }
    void Script::v_mirror(){
        for(int y = 0; y< image->height()/2; y++){                //Uma vez que o objetivo é espelhar verticalmente uma imagem, não necessitamos de iterar sobre todos os píxeis de uma imagem;
            for(int x = 0; x< image->width(); x++){           //De facto, ao utilizarmos estes 2 ciclos "for" iremos apenas iterar os pixeis até metade da altura da imagem;                //A variável "pixel5" irá guardar o valor do píxel do seu equivalente ao espelhar a imagem verticalmente("pixel6");
                Color temporaria = image->at(x,y);      //A variável "pixel6" irá guardar e fornecer a "pixel5" a cor que o píxel deve tomar para tornar a espelhagem vertical possível;
                image->at(x,y) = image->at(x,image->height()-1- y); 
               image->at(x,image->height()-1- y) = temporaria;
            }
        }
    }
    void Script::add() {
        std::string filename;
        int r, g, b, x, y;
        input >> filename >> r >> g >> b >> x >> y;
        Image* imagemnova = loadFromPNG(filename);

        for (int i = 0; i < imagemnova->height(); i++) {
            for (int j = 0; j < imagemnova->width(); j++) {
                Color pixelnovo = imagemnova->at(j, i);
                if (pixelnovo.red() != r || pixelnovo.green() != g || pixelnovo.blue() != b) {
                    Color& pixel2 = image->at(x+j, y+i);
                    pixel2.red() = pixelnovo.red();
                    pixel2.green() = pixelnovo.green();
                    pixel2.blue() = pixelnovo.blue();
                }
            }
        }
        delete imagemnova;
    }
    void Script::rotate_left(){
        std::vector<Color> imagem_rodada_esquerda;
        imagem_rodada_esquerda = std::vector<Color>(image->height() * image->width());  //Criação de um vetor que, futuramente, irá conter a imagem rodada;
        for(int x = 0; x < image->width(); x++){                                        //Através destes dois ciclos "for", iremos iterar sobre todos os píxeis da imagem inicial;
            for(int y = 0; y< image->height(); y++){    
                imagem_rodada_esquerda[(image->width()-1-x) * image->height() + y] = image->at(x, y);
            }
        }
        
        int temporaria = image->width();
        image->width() = image->height();
        image->height() = temporaria;

        for(int x = 0; x < image->width(); x++){
            for(int y = 0; y < image->height(); y++){
                image->at(x, y) = imagem_rodada_esquerda[y * image->width() + x];
            }   
        }
    }
    void Script::rotate_right(){
        std::vector<Color> imagem_rodada_direita;
        imagem_rodada_direita = std::vector<Color>(image->height() * image->width());

        for(int x = 0; x < image->width(); x++){
            for(int y = 0; y < image->height(); y++){
                imagem_rodada_direita[(x+1) * image->height() - y - 1] = image->at(x, y);
            }
        }

        int temp = image->width();
        image->width() = image->height();
        image->height() = temp;

        for(int x = 0; x < image->width(); x++){
            for(int y = 0; y < image->height(); y++){
                image->at(x, y) = imagem_rodada_direita[y * image->width() + x];
            }
        }
    }
    void Script::crop(){
        int x, y, w,h;
        input >> x >> y >> w >> h;
        Image* imagemnova = new Image(w, h);
        for (int i = x; i < x + w; i++) {
            for (int j = y; j < y + h; j++) {
                imagemnova->at(i - x, j - y) = image->at(i, j);
            }
        }
        delete image;
        image = imagemnova;
    }
    void Script::median_filter(int ws){
        Image imagemnova(image->width(), image->height());  //Através da criação de uma nova imagem "imagemnova", com as dimensões da imagem original, iremos armazenar os píxeis da imagem original na qual já está aplicada a mediana;

        for (int y = 0; y < image->height(); y++) {         //Ao utilizarmos estes dois ciclos "for" iremos iterar sobre todos os píxeis de uma imagem;
            for (int x = 0; x < image->width(); x++) {
                int minX = std::max(0, x - ws/2);           //Ao definirmos minX, minY, maxX e maxY, iremos fornecer a cada uma destas variáveis os limites das dimensões sobre os quais podem iterar ao longo da imagem;
                int minY = std::max(0, y - ws/2);           //É importante notar que estes valores são fornecidos no enunciado;
                int maxX = std::min(image->width() - 1, x + ws/2);
                int maxY = std::min(image->height() - 1, y + ws/2);

                std::vector<rgb_value> vermelhos, verdes, azuis;    //Criação de 3 vetores que armazenarão variáveis do tipo "rgb_value", onde cada um guardará as variáveis com os valores de cada porção da cor RGB;

                for (int ny = minY; ny <= maxY; ny++) {                     //Ao utilizarmos dois loops "for" iremos iterar sobre os limites definidos a cima;
                    for (int nx = minX; nx <= maxX; nx++) {                 //Nestes ciclos iremos percorrer os píxeis dentro dos limites propostos;
                        vermelhos.push_back(image->at(nx, ny).red());       //Iremos ao píxel em questão, ou seja, o píxel situado em (nx,ny) e vamos adicionar a sua parte "Vermelha" ao vetor que armazena esses dados("vermelhos");
                        verdes.push_back(image->at(nx, ny).green());        //Iremos ao píxel em questão, ou seja, o píxel situado em (nx,ny) e vamos adicionar a sua parte "Verde" ao vetor que armazena esses dados ("verdes");
                        azuis.push_back(image->at(nx, ny).blue());     //Iremos ao píxel em questão, ou seja, o píxel situado em (nx,ny) e vamos adicionar a sua parte "Azul" ao vetor que armazena esses dados ("azuis");
                    }
                }

                std::sort(vermelhos.begin(), vermelhos.end());      //Iremos dar "sort" ao vetor que guarda o valor RGB "vermelho" dos pontos;
                std::sort(verdes.begin(), verdes.end());            //Iremos dar "sort" ao vetor que guarda o valor RGB "verde" dos pontos;
                std::sort(azuis.begin(), azuis.end());              //Iremos dar "sort" ao vetor que guarda o valor RGB "azul" dos pontos;

                rgb_value medianI;                                  //Definição de 4 variáveis do tipo rgb; medianI vai armazena o índice o valor da mediana de todos os valores;
                rgb_value medianR;                                  //medianR irá guardar o valor da mediana do vetor "vermelhos";
                rgb_value medianG;                                  //medianG irá guardar o valor da mediana do vetor "verdes";
                rgb_value medianB;                                  //medianB irá guardar o valor da mediana do vetor "azulis";
                if (vermelhos.size()%2==1){                         //Nesta condição verificamos se o número de elementos dos vetores é impar ou é par;
                    medianI = (vermelhos.size() - 1) / 2;           //Sendo impar, a medianI é calculado apenas pela divisão do tamanho do vetor menos 1;
                    medianR = vermelhos[medianI];                   //Atribuição dos valores de medianR, medianG e medianB através do indíce fornecido por medianI;
                    medianG = verdes[medianI];
                    medianB = azuis[medianI];
                }
                else{                                                                                           //Caso o número de elementos dos vetores seja par, o indíce será calculado de maneira diferente;
                    medianR = (vermelhos[(vermelhos.size()/2) -1] + vermelhos[vermelhos.size() / 2])/2;         //Atribuição dos valores de medianR, medianG, medianB através da média entre os dois elementos do meio de um vetor;
                    medianG = (verdes[(vermelhos.size()/2) -1] + verdes[vermelhos.size() / 2])/2;
                    medianB = (azuis[(vermelhos.size()/2) -1] + azuis[vermelhos.size() / 2])/2;
                }
                imagemnova.at(x, y).red() = medianR;                //Finalmente, iremos ao ponto em questão e iremos substituir cada uma das suas porções RGB por a mediana das mesmas;
                imagemnova.at(x, y).green() = medianG;
                imagemnova.at(x, y).blue() = medianB;
            }
        }

        for (int y = 0; y < image->height(); y++) {                 //Para finalizar resta apenas voltar a copiar os píxei de "imagemnova" criada para a imagem original;
            for (int x = 0; x < image->width(); x++) {              //Deste modo, iremos fazer isso através destes dois loops "for", nos quais iremos iterar sobre todos os pontos;
                image->at(x, y).red() = imagemnova.at(x, y).red();  //A cada ponto iterado iremos copiar as suas vertentes RGB para o píxel da imagem principal;
                image->at(x, y).green() = imagemnova.at(x, y).green();
                image->at(x, y).blue() = imagemnova.at(x, y).blue();
            }
        }
    }
}