	section 	test,$0000
	org	$0000
	rldi	rc,$ffff

test:	rldi	ra,$0000
	rldi	rb,$0000

wait:	inc	ra
	b2	wait
	
wait1:	inc	rb
	bn2	wait1
	
	dbnz	rc,test
	
	end
