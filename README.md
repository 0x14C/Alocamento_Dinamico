# Alocamento_Dinamico
Esse projeto tem o intuito para colocar em prática a teoria aprendida sobre espaço de endereçamento mais especialmente o segmento da memória  HEAP e como funciona a expansão/redimensionamento da heap quando o processo solicita alocamento de memória naquele espaço de endereço. 

Mas a final, o que seria heap na memória virtual? 
A heap ela faz parte da memória virtual, e o principal objetivo da heap é alocar memória para processos que só terão informações em tempo de execução e que seria inviável alocar um espaço de memória estaticamente pois você poderia consumir mais memória do que o necessário. Com o segmento HEAP podemos alocar a quantidade necessária e assim economizar mais memória. Vendo a imagem abaixo podemos ter uma noção dos segmentos que contém o espaço de endereçamento de um processo(programa em execução):

![image](https://github.com/user-attachments/assets/21a9039c-f917-463e-aa51-c2e0f436cb68)

A heap ela cresce de baixo para cima conforme podemos visualizar na imagem acima, e todo alocamento de memória que ela executar, será de forma contígua(conforme na imagem unallocated memory), normalmente esse espaço também é usado pelas DLL mas no final esses endereço de memória virtual(referente os objetos ou estrutura que são criada dinamicamente para as funções da dll) serão mapeado para as páginas fisicas da memória ram(onde realmente o modulo daquela biblioteca está sendo ocupado) e cabe a MMU fazer esse mapeamento através da tabela de paginação. Depois dessa pequena e muito breve teoria da região HEAP da memória, deixarei em anexo um projeto prático que fiz criando a minha própria função de alocamento dinamico, o codigo se encontra documentado.
