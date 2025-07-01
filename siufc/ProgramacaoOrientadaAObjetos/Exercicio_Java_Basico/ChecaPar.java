import java.util.Scanner;

public class ChecaPar{
public static void main (String args []){
	
int num = 0;

System.out.println("Digite um número");

Scanner sc = new Scanner(System.in);

num = sc.nextInt();

Boolean verifica = num % 2 == 0 ? true : false;

System.out.println(verifica);

}
} 