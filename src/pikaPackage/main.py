class Env:
    # def __getattr__(self, item):
        # return self[item]
    ...

env = Env()
env.a = 1
print(env.a)
