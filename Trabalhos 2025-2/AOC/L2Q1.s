# Faça um programa em Assembly para MIPS que:

# Leia dois inteiros n e k da entrada padrão, respectivamente. Um em cada linha.
# n é um inteiro positivo de 32 bits em complemento 2 0 <= k <= 9
# Seu programa deve tratar uma entrada composta por vários casos de testes e parar quando: n == 0 (sem ler o respectivo k)
# Implemente uma função recursiva que determine quantas vezes 0 dígito K ocorre em um número natural N
# Imprima uma linha com o resultado retornado pela função na saída padrão
# A função recursiva deve fazer apenas o cálculo (salvando e restaurando o contexto como apropriado)
# A função principal (main) deve fazer toda a parte de entrada e saída
# Respeite a convenção dos registradores para receber e retornar valores de função

	.text
	.globl main
main:
	li $v0, 5
	syscall
	ori $a0, $v0, 0 #contém o número que eu quero ver as repetições
	beqz $a0, end

	li $v0, 5
	syscall
	ori $a1, $v0, 0 #contém o número que eu quero achar

	jal contaDig
	move $a0, $v0
	li $v0, 1
	syscall
	li $a0, 10 #código do '\n'
	li $v0, 11 #código para printar $a0 como char
	syscall
	j main
end:
	li $v0, 10
	syscall
contaDig:
	#condição de parada
	beqz $a0, contaDig_parada
	
	#salvo valores
	addi $sp, $sp, -8
	sw $s0, 0($sp)
	sw $ra, 4($sp)
	
	#operações
	li $t1, 10
	div $a0, $t1
	mflo $a0 #movo o quociente para $a0
	mfhi $t0 #movo o restante para $t0
	seq $s0, $t0, $a1 #se $t0 == $a1, então $t0 = 1, se não, $t0 = 0
	jal contaDig
	add $v0, $v0, $s0
	
	#recarrego informações
	lw $s0, 0($sp)
	lw $ra, 4($sp)
	addi $sp, $sp, 8
	
	jr $ra

contaDig_parada:
	li $v0, 0
	jr $ra