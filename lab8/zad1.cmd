**Domyslne wartości dla:**
**1.Liczby istniejących kolejek komunikatów** - 256 (cat /proc/sys/fs/mqueue/queues_max)
**2.Maksymalnej (w skali systemu) liczby komunikatów w kolejce** - 10 (cat /proc/sys/fs/mqueue/msg_max)
**3.Maksymalnego (w skali systemu) rozmiaru komunikatu** - 8kB (cat /proc/sys/fs/mqueue/msgsize_max)
**4.Domyślnej maksymalnej liczby komunikatów w nowo tworzonej kolejce** - Brak globalnej domyślnej; wartość jest podawana przy tworzeniu kolejki (mq_maxmsg). Jeśli nie zostanie określona, użyta jest wartość z msg_max (punkt 2)
**5.Domyślnego maksymalnego rozmiaru komunikatu w nowo tworzonej kolejce** - Brak globalnej domyślnej; wartość jest podawana przy tworzeniu kolejki (mq_msgsize). Jeśli nie zostanie określona, użyta jest wartość z msgsize_max (punkt 3)