# Faça um programa em Assembly para MIPS que:
# Solicita um valor inteiro n ao usuário da entrada padrão
# Você pode assumir que 0 <= n <= 10
# Implemente uma função recursiva que calcula o n-ésimo número de Fibonacci
# Considere que para n == 0 e n == 1 o resultado é 1 e para n == 2 o resultado é 2.
# Imprime na saída padrão uma linha com o resultado
# A função recursiva deve fazer apenas o cálculo (salvando e restaurando o contexto como apropriado)
# A função principal (main) deve fazer toda a parte de entrada e saída
# Respeite a convenção dos registradores para receber e retornar valores de função

	.text
	.globl main
main:
	li $v0, 5
	syscall
	ori $a0, $v0, 0 
	jal fib
	
	move $a0, $v0
	li $v0, 1
	syscall
end:
	li $v0, 10
	syscall
fib:
	# critério de parada
	blt $a0, 2, fib_parada
	
	# salvar ctx
	addi $sp, $sp, -12
	sw $s0, 0($sp)
	sw $ra, 4($sp)
	sw $s1, 8($sp)
	
	# código principal
	# return fib(a0-1) + fib(a0-2)
	move $s1, $a0
	addi $a0, $s1, -1
	jal fib
	move $s0, $v0
	addi $a0, $s1, -2
	jal fib
	add $v0, $s0, $v0 #v0 já contém o resultado de fib(a0-2)
	
	# restaurar ctx
	lw $s0, 0($sp)
	lw $ra, 4($sp)
	lw $s1, 8($sp)
	addi $sp, $sp, 12
	
	# retornar
	jr $ra
fib_parada:
	li $v0, 1
	jr $ra