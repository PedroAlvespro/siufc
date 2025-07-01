import java.util.Scanner; // Importação do Scanner para entrada de dados

public class Verifica {
    public static void main(String[] args) { // Método main necessário para a execução do código
        int numero = 50; // Número fixo para comparação
        int resposta; // Variável que armazenará a resposta do usuário

        Scanner scanner = new Scanner(System.in); // Criação do Scanner para entrada de dados

        System.out.println("Digite um valor entre 0 e 50"); // Exibe a mensagem para o usuário
        resposta = scanner.nextInt(); // Lê a entrada do usuário

        // Verifica se o valor digitado está dentro do intervalo permitido
        if (resposta > 50 || resposta < 0) {
            System.out.println("Valor inválido! Digite um valor entre 0 e 50.");
        } else if (resposta < 25) {
            System.out.println("Quase lá, maior que " + resposta);
        } else if (resposta < 40) {
            System.out.println("Quase lá, maior que " + resposta);
        } else if (resposta < 45) {
            System.out.println("Quase lá, maior que " + resposta + ". Você está quase lá, apenas mais alguns números.");
        } else if (resposta == numero) {
            System.out.println("Parabéns! Você acertou.");
        } else {
            System.out.println("Tente novamente!");
        }

        scanner.close(); // Fecha o Scanner para evitar vazamento de memória
    }
}
