/*ex 3
Criar um programa em Java com blocos independentes de código (switch case). O programa deve obedecer a seguinte ordem para cada opção numérica fornecida como entrada:

1. Leitura e classificação de um número:

Leia um número inteiro.
Se o número for maior que 10, imprima "MAIOR".
Caso contrário, imprima "MENOR".
2. Manipulação de um número:

Leia um número inteiro.
Se for múltiplo de 2:
Multiplique o número por 5.
Se for múltiplo de 3:
Subtraia 1 do número.
 
Se não for múltiplo de 2 e nem de 3:
Não faça nada.
Imprima o resultado final do número.
3. Leitura de palavras:

Leia um número inteiro positivo.
Leia uma quantidade de palavras (String) igual ao numero informado.
Imprima as palavras separadas por espaço, da última para a primeira.
4. Cálculo do Máximo Divisor Comum (MDC):

Leia dois números inteiros positivos.
Calcule o MDC entre os dois números.
Imprima o MDC.
5. Soma de números:

Leia vários números inteiros até realizar a leitura de um não número.
Calcule a soma de todos os números lidos.
Imprima a soma final.
Observações:

Todas as impressões devem pular linha.
Caso um número seja informado ao switch case fora do intervalo permitido, o programa deve simplesmente encerrar a execução. 
*/
import java.util.Scanner;

public class main3{
	public static void main(String[] args) {
	Scanner caso = new Scanner(System.in);
	
	System.out.println("Escolha entre as opções (1-5) :");
	int escolha = caso.nextInt();
	
	switch(escolha){
	    case 1:
	        System.out.println("Digite um número inteiro: ");
	        int inteiro = caso.nextInt();
	        
	        if(inteiro >10)
	        System.out.println("MAIOR");
	        else 
	        System.out.println("MENOR");
	         break;
	     
	     case 2:
	        System.out.println("Digite um número inteiro: ");
	        int inteiro2 = caso.nextInt();
	        
	        if(inteiro2% 2 ==0)
	        inteiro2 = inteiro2 * 5;
	        if(inteiro2% 3 ==0)
	        inteiro2--;
	        
	        System.out.println("Resultado : " + inteiro2);
	        break;
	        
	        
	     case 3:
	         System.out.println("Digite o número de palavras: ");
	         int numero = caso.nextInt();
	         
	         String[] s = new String[numero];
	         caso.nextLine();
	         
	         System.out.println("Digite as " + numero + " palavra(s)");
	         for(int i=0; i<numero; i++){
	             s[i] = caso.nextLine();
	         }
	         for(int i = numero -1; i>=0; i--){
	         System.out.print(s[i] + " ");
	         }
	         System.out.println();
	         break;
	         
	         
	      case 4:
	          System.out.println("Digite o primeiro número inteiro: ");
	          int val1 = caso.nextInt();
	          System.out.println("Digite o segundo número inteiro: ");
	          int val2 = caso.nextInt();
	          
	          while(val2 !=0){
	              int troca = val2;
	              val2 = val1 %val2;
	              val1 = troca;
	          }
	          System.out.println("MDC: " + val1);
	          break;
	          
	          
	       case 5:
	           System.out.println("Digite números inteiros, caso queira parar digite algo não numérico");
	           int S =0;
	           while(caso.hasNextInt()){
	              S+= caso.nextInt(); 
	           }
	           System.out.println("Somatório = " + S);
	           break;
	           
	    default:
	    System.out.println("Encerrando :D");
	}
	caso.close();
	}
}
