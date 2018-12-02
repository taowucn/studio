
.global main

.text

main:
	stmfd sp!, {r11, lr}
	mov r0, #5
	ldmfd sp!, {r11, pc}

.end
