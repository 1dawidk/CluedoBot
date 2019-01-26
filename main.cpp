#include <iostream>
#include <ncurses.h>
#include "CluedoCard.h"
#include "UI.h"
#include "CluedoProgress.h"

/**
 * Bot gry Cluedo który na podstawie zadawanych pytań i informacji o
 * tym kto pokazał kartę do danego pytania ustala kto jest w magicznej
 * kopercie.
 *
 * Karty podzielone są na trzy grupy:
 *      - Postaci (index 0)
 *      - Broń    (index 1)
 *      - Miejsca (index 2)
 *
 * Grupa         Nazwa karty      index
 *
 * Postaci:     + Zielnicki         0
 *              + Gorczynski        1
 *              + Kostkiewicz       2
 *              + Purpulewski       3
 *              + Gwiazdowicz       4
 *              + Siwecka           5
 *
 * Broń:        + Klucz francuski   0
 *              + Swiecznik         1
 *              + Sztylet           2
 *              + Rewolwer          3
 *              + Olowiana rura     4
 *              + Lina              5
 *
 * Miejsca:     + Lazienka          0
 *              + Gabinet           1
 *              + Jadalnia          2
 *              + Pokoj bilardowy   3
 *              + Garaz             4
 *              + Sypialnia         5
 *              + Salon             6
 *              + Kuchnia           7
 *              + Hol               8
 *
 *
 * Dane zbierane jako wpisy:
 *      - jakie kart są znane wszystkim
 *      - jakie karty posiadam w ręce
 *      - pytań i odpowiedzi:
 *          + jakich kart dotyczylo pytanie
 *          + kto pokazal karte
 *
 * Wynik jest reprezentowany jako wartość prawdopodobieństwa wystąpienia
 * danej karty w magicznej kopercie
 *
 * Do wynaczania tego prawdopodobieństwa użyta jest teoria Demstera-Schafera w celu
 * określenia prawdopodobieństw posiadania danych kart przez kolejnych graczy.
 *
 * Jeżeli np pytanie dotyczyło kart 1, 1, 4 i odpowiedział gracz 2 to twierdzimy że
 * z pstwem 0.333 gracz 2 posiada jedną z tych kart jest prawdą natomiast dla pozostałych
 * graczy twierdzimy że z pewnym pstwem NIE posiadają jednej z kart.
 */

int main() {
    UI ui;

    ui.init();

    while(ui.getState()!=UI::Quit){
        ui.onLoop();
    }


    ui.end();
    return 0;
}