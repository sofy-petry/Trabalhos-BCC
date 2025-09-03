/*ex 2.4
Desenvolva um programa em Java capaz de imprimir os números da sequência de Fibonacci até 100 (não é para imprimir os cem primeiros números da sequência, apenas é para imprimir os números até o próximo ultrapassar cem). Os números devem ser separados por espaços, e não deve haver quebra de linha entre eles.
*/
public class main24{
    public static void main(String agrs[]){
        int a=0, b=1, agora=0;
        System.out.print(a + " " + b );
        while (agora<100){
            agora = a+b;
            if(agora>100) 
            break;
            System.out.print(" " + agora);
            a = b;
            b = agora;
        }
    }
}