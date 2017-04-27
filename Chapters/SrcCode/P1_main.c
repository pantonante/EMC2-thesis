extern int main(void){
	P4_uid_t me;
	P4_e_t rc;
	P4_thr_t thread_num[TNUM];
    
	/* Spinlocks Initialization */
	p4_spin_init(&AttitudeLoop1_ControlMixer1_spinlock);
	p4_spin_init(&AttitudeLoop2_ControlMixer2_spinlock);
	p4_spin_init(&HeightLoop1_GravityFeedforward1_spinlock);
	p4_spin_init(&PositionLoop1_PositionRate1_spinlock);
	p4_spin_init(&PositionLoop2_PositionRate2_spinlock);
	p4_spin_init(&YawLoop1_invRZ1_spinlock);

	/* Lower main's prio in order to let threads creation */
	rc = p4_thread_set_priority(P4_THREAD_MYSELF, 1);
	ASSERT(rc == P4_E_OK, "Main->p4_thread_set_priority() failed with rc == 0x%02x", rc);

	/* Params: Tnumber, prio, affinitiMask, name, entryPt */
	initThread((P4_thr_t*)&thread_num[0], 7, P4_CPUMASK_CPU_TO_MASK(0), "AttitudeLoop", AttitudeLoop_thread);
	initThread((P4_thr_t*)&thread_num[1], 5, P4_CPUMASK_CPU_TO_MASK(1), "ControlMixer", ControlMixer_thread);
	initThread((P4_thr_t*)&thread_num[2], 6, P4_CPUMASK_CPU_TO_MASK(1), "GravityFeedforward", GravityFeedforward_thread);
	initThread((P4_thr_t*)&thread_num[3], 8, P4_CPUMASK_CPU_TO_MASK(0), "HeightLoop", HeightLoop_thread);
	initThread((P4_thr_t*)&thread_num[4], 7, P4_CPUMASK_CPU_TO_MASK(1), "PositionLoop", PositionLoop_thread);
	initThread((P4_thr_t*)&thread_num[5], 5, P4_CPUMASK_CPU_TO_MASK(0), "PositionRate", PositionRate_thread);
	initThread((P4_thr_t*)&thread_num[6], 8, P4_CPUMASK_CPU_TO_MASK(1), "YawLoop", YawLoop_thread);
	initThread((P4_thr_t*)&thread_num[7], 6, P4_CPUMASK_CPU_TO_MASK(0), "invRZ", invRZ_thread);

	/* Once thread are created (so they are in STOPPED STATE) main thread can escalate its priority */
	rc = p4_thread_set_priority(P4_THREAD_MYSELF, TASK_HIGH_PRIO);
	ASSERT(rc == P4_E_OK, "Main->p4_thread_set_priority() failed with rc == 0x%02x", rc);

	vm_cprintf("\nALL THREADS INITIALIZED\n");
	vm_cprintf("------------------------------------------------------------------\n");

    /* Main loop */
	for(;;) {
     	/* RESUME THREADS*/
		for (int i=0; i<TNUM; i++)
			resumeTask(i, thread_num);

		/* WAIT FOR NEXT PERIOD */
		p4_sleep(P4_TIMEOUT_TP_PERIOD | P4_TIMEOUT_INFINITE);
	}
}