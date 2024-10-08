Launching with Slurm
====================

Open MPI supports two modes of launching parallel MPI jobs under
Slurm:

#. Using Open MPI's full-features ``mpirun`` launcher.
#. Using Slurm's "direct launch" capability.

Unless there is a strong reason to use ``srun`` for direct launch, the
Open MPI team recommends using ``mpirun`` for launching under Slurm jobs.

.. note:: In versions of Open MPI prior to 5.0.x, using ``srun`` for
   direct launch could be faster than using ``mpirun``.  **This is no
   longer true.**

Using ``mpirun``
----------------

When ``mpirun`` is launched in a Slurm job, ``mpirun`` will
automatically utilize the Slurm infrastructure for launching and
controlling the individual MPI processes.
Hence, it is unnecessary to specify the ``--hostfile``,
``--host``, or ``-np`` options to ``mpirun``.

.. note:: Using ``mpirun`` is the recomended method for launching Open
   MPI jobs in Slurm jobs.

   ``mpirun``'s Slurm support should always be available, regardless
   of how Open MPI or Slurm was installed.

For example:

.. code-block:: sh

   # Allocate a Slurm job with 4 slots
   shell$ salloc -n 4
   salloc: Granted job allocation 1234

   # Now run an Open MPI job on all the slots allocated by Slurm
   shell$ mpirun mpi-hello-world

This will run the 4 MPI processes on the node(s) that were allocated
by Slurm.

Or, if submitting a script:

.. code-block:: sh

   shell$ cat my_script.sh
   #!/bin/sh
   mpirun mpi-hello-world
   shell$ sbatch -n 4 my_script.sh
   srun: jobid 1235 submitted
   shell$

Similar to the ``salloc`` case, no command line options specifing
number of MPI processes were necessary, since Open MPI will obtain
that information directly from Slurm at run time.

Using Slurm's "direct launch" functionality
-------------------------------------------

Assuming that Slurm installed its Open MPI plugin, you can use
``srun`` to "direct launch" Open MPI applications without the use of
Open MPI's ``mpirun`` command.

.. note:: Using direct launch can be *slightly* faster when launching
   very, very large MPI processes (i.e., thousands or millions of MPI
   processes in a single job).  But it has significantly fewer
   features than Open MPI's ``mpirun``.

First, you must ensure that Slurm was built and installed with PMI-2
support.

.. note:: Please ask your friendly neighborhood Slurm developer to
          support PMIx.  PMIx is the current generation of run-time
          support API; PMI-2 is the legacy / antiquated API.  Open MPI
          *only* supports PMI-2 for Slurm.

Next, ensure that Open MPI was configured ``--with-pmi=DIR``, where
``DIR`` is the path to the directory where Slurm's ``pmi2.h`` is
located.

Open MPI applications can then be launched directly via the ``srun``
command.  For example:

.. code-block:: sh

   shell$ srun -N 4 mpi-hello-world

Or you can use ``sbatch`` with a script:

.. code-block:: sh

   shell$ cat my_script.sh
   #!/bin/sh
   srun mpi-hello-world
   shell$ sbatch -N 4 my_script.sh
   srun: jobid 1235 submitted
   shell$

Similar using ``mpirun`` inside of an ``sbatch`` batch script, no
``srun`` command line options specifing number of processes were
necessary, because ``sbatch`` set all the relevant Slurm-level
parameters about number of processes, cores, partition, etc.

Slurm 20.11
-----------

There were some changes in Slurm behavior that were introduced in
Slurm 20.11.0 and subsequently reverted out in Slurm 20.11.3.

SchedMD (the makers of Slurm) strongly suggest that all Open MPI users
avoid using Slurm versions 20.11.0 through 20.11.2.

Indeed, you will likely run into problems using just about any version
of Open MPI these problematic Slurm releases.

.. important:: Please either downgrade to an older version or upgrade
               to a newer version of Slurm.
