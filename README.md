# Dining-Philosophers
A simple yet robust solution to Dining Philosophers in pthread
## Output format
Philo   State                   Fork    Held by<br>
[ 0]:   Thinking                [ 0]:   Free<br>
[ 1]:   Eating                  [ 1]:      1<br>
[ 2]:   Waiting                 [ 2]:      1<br>
[ 3]:   Terminated              [ 3]:      2<br>
[ 4]:   Eating                  [ 4]:      4<br>
[ 5]:   Thinking                [ 5]:      4<br>
[ 6]:   Thinking                [ 6]:   Free<br>
[ 7]:   Thinking                [ 7]:   Free<br>
[ 8]:   Eating                  [ 8]:      8<br>
[ 9]:   Thinking                [ 9]:      8<br>
Th= 5 Wa= 1 Ea= 3               Use= 7  Avail= 3<br>

### Usage
 - Compilation: ` gcc DPP.c -o DPP -Wall -pthread`
 - Run: `./DPP <N> <S> <T>`<br>
   N = number of philosophers<br>
   S = random number seed<br>
   T = seconds of running<br>
