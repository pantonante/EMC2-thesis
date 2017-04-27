int main(void){
    P4_thr_t thread_num[TNUM];

    /* Lower main's prio in order to let threads creation */
	p4_thread_set_priority(P4_THREAD_MYSELF, 1);
	
	/* For each child thread */
	for(int i=0; i<TNUM; i++)
		initThread((P4_thr_t*)&thread_num[i], %<Priority>, P4_CPUMASK_CPU_TO_MASK(<AffinityMask>), "<Name>", <Prototype>);
    
	/* Once thread are created (so they are in STOPPED STATE) main thread can escalate its priority */
	rc = p4_thread_set_priority(P4_THREAD_MYSELF, TASK_HIGH_PRIO);

    /* Main loop */
	for(;;) {
		/* RESUME THREADS*/
		for (int i=0; i<TNUM; i++)
			resumeTask(i, thread_num);

		/* WAIT FOR NEXT PERIOD */
		p4_sleep(P4_TIMEOUT_TP_PERIOD | P4_TIMEOUT_INFINITE);
    }
}