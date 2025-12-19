
#Faça um programa em Assembly para MIPS que:
# Leia um inteiro n da entrada padrão
#Você pode assumir que 1 <= n < 10
# Calcule n!
# Imprima uma linha com o resultado na saída padrão


	.text
	.globl main 
main:
	li $v0, 5 
	syscall 
	move $s1, $v0 
	li $s0, 1 
while:   
	mult $s0, $s1 
	mflo $s0 
	add $s1, $s1, -1 
	bnez $s1, while 
	move $a0, $s0 
	li $v0, 1 
	syscall
end:
	li $v0, 10 
	 syscall 
	 
	 