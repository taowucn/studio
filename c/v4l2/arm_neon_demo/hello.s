	.syntax unified
	.arch armv7-a
	.eabi_attribute 27, 3
	.eabi_attribute 28, 1
	.fpu neon
	.eabi_attribute 20, 1
	.eabi_attribute 21, 1
	.eabi_attribute 23, 3
	.eabi_attribute 24, 1
	.eabi_attribute 25, 1
	.eabi_attribute 26, 2
	.eabi_attribute 30, 6
	.eabi_attribute 34, 1
	.eabi_attribute 18, 4
	.thumb
	.file	"hello.c"
	.text
	.align	2
	.global	main
	.thumb
	.thumb_func
	.type	main, %function
main:
	@ args = 0, pretend = 0, frame = 16
	@ frame_needed = 1, uses_anonymous_args = 0
	@ link register save eliminated.
	push	{r7}
	sub	sp, sp, #20
	add	r7, sp, #0
	movw	r3, #39322
	movt	r3, 16281
	str	r3, [r7, #12]	@ float
	movw	r3, #26214
	movt	r3, 16294
	str	r3, [r7, #8]	@ float
	flds	s14, [r7, #12]
	flds	s15, [r7, #8]
	fdivs	s15, s14, s15
	fsts	s15, [r7, #4]
	movs	r3, #0
	mov	r0, r3
	adds	r7, r7, #20
	mov	sp, r7
	@ sp needed
	ldr	r7, [sp], #4
	bx	lr
	.size	main, .-main
	.ident	"GCC: (crosstool-NG -  Linaro Multilib GCC [CortexA9]"
	.section	.note.GNU-stack,"",%progbits
