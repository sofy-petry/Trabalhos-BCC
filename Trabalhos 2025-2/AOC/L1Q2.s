
#Faça um programa em Assembly para MIPS que:
# Solicita continuamente valores inteiros ao usuário da entrada padrão
# O programa termina quando o usuário digita -1
#Você pode assumir que todos os números informados são inteiros maiores ou igual a -1
# Calcula a soma e a média dos valores solicitados, (desconsiderar o -1 digitado para terminar)
# Para a média faça a divisão inteira da soma com a quantidade de valores digitados
# Ao final o programa deve exibir (imprimir) duas linhas na saída padrão:
# Na primeira a soma
# Na segunda linha a média dos valores digitados
# Se for uma divisão por zero (nenhum valor digitado) não imprima o resultado da divisão e sim a mensagem "ERRO"
# A soma de nenhum número resulta em zero
# É garantido que o total e subtotais na sequencia informada pode ser armazenado em um inteiro de 32 bits usando complemento 2.


.data
	erro: .asciiz "ERRO\n"
	quebra: .asciiz "\n"
	
.text
	.globl main

main:
	li $s0, 0
	li $t0, 0
	
loop:
	li $v0, 5
	syscall
	
	move $s1, $v0
	beq $s1, -1, fim
	
	add $s0, $s0, $s1
	addi $t0, $t0, 1
	j loop

fim:
	li $v0, 1
	move $a0, $s0
	syscall
	
	li $v0, 4
	la $a0, quebra
	syscall
	
	beq $t0, $zero, _erro
	div $s0, $t0
	mflo $a0
	li $v0, 1
	syscall
	
	j finaliza_programa
	
_erro:
	li $v0, 4
	la $a0, erro
	syscall
	
finaliza_programa:
	li $v0, 10
	syscall