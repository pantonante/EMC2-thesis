static void thread(void){
	/* Subsystem Initialize entry function */
    subsystem_initialize();
	
	/* Wait for the first activatio */
	p4_thread_stop(P4_THREAD_MYSELF);

    /* Continous loop */
    for(;;){
		/* Get predecessor output */
		subsystem_getInput();

        /* Step */
        subsystem_step();
        
        /* Wait for next period */
		p4_thread_stop(P4_THREAD_MYSELF);
	}

    /* Terminate - Never Reached */
    subsystem_terminate();
}