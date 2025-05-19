import java.util.Scanner;

public class Advinha2 {
	
	public static void main (String args[]){

	int num = 20;
	int resposta;

	Scanner cs = new Scanner(System.in);

	if(num > 20 || num < 0){
	System.out.println("Impossível");
} else if(num < 15){
	System.out.println("um pouco mais");
} else {
	System.out.println("Entrada inválida");
}
}

}