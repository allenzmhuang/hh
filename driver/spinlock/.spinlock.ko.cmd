cmd_/home/gec/exercise/driver/spinlock/spinlock.ko :=  ld -r -m elf_i386 -T ./scripts/module-common.lds --build-id  -o /home/gec/exercise/driver/spinlock/spinlock.ko /home/gec/exercise/driver/spinlock/spinlock.o /home/gec/exercise/driver/spinlock/spinlock.mod.o ;  true