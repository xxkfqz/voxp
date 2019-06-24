import os

PACKAGE_NAME = 'voxp'

env = Environment(
	CCFLAGS = [
		'-DPACKAGE_NAME = \\"{}\\"'.format(PACKAGE_NAME),
		'--std=c11',
		'-O3',
		'-I./include/'
	],

	LINKFLAGS = [
		'-ldl',
		'-lm'
	]
)

# Color output
env['ENV']['TERM'] = os.environ['TERM']

Export('env')

env.Program(PACKAGE_NAME, Glob('src/*.c'))
