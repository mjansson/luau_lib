#!/usr/bin/env python

"""Ninja build configurator for luau library"""

import sys
import os

sys.path.insert(0, os.path.join('build', 'ninja'))

import generator

dependlibs = ['luau', 'foundation']
extralibs = []
extravariables = {}

generator = generator.Generator(project = 'luau', dependlibs = dependlibs, variables = [('bundleidentifier', 'com.maniccoder.luau.$(binname)')])
target = generator.target
writer = generator.writer
toolchain = generator.toolchain

if target.is_windows():
  extralibs += ['gdi32', 'ws2_32', 'iphlpapi']
if target.is_linux():
  extralibs += ['X11', 'Xext', 'GL']

luau_lib = generator.lib(module = 'luau', sources = [
  'luau.c', 'version.c'] +
  [os.path.join('luau', 'VM', 'src', item) for item in [
    'lapi.c', 'laux.c', 'lbaselib.c', 'lbitlib.c', 'lbuiltins.c', 'lcorolib.c', 'ldblib.c', 'ldebug.c', 'ldo.c', 'lfunc.c', 'lgc.c', 'lgcdebug.c',
    'linit.c', 'lmathlib.c', 'lmem.c', 'lnumprint.c', 'lobject.c', 'loslib.c', 'lperf.c', 'lstate.c', 'lstring.c', 'lstrlib.c', 'ltable.c',
    'ltablib.c', 'ltm.c', 'ludata.c', 'lutf8lib.c', 'lvmexecute.c', 'lvmload.c', 'lvmutils.c']],
  includepaths = [os.path.join('luau', 'luau', 'VM', 'include'), os.path.join('luau', 'luau', 'Common', 'include')])

#if not target.is_ios() and not target.is_android():
  #configs = [config for config in toolchain.configs if config not in ['profile', 'deploy']]
  #if not configs == []:
    #generator.bin('luau', ['main.c'], 'luau', basepath = 'tools', implicit_deps = [luau_lib], dependlibs = dependlibs, libs = extralibs, configs = configs, variables = extravariables)
    #generator.bin('luaudump', ['main.c'], 'luaudump', basepath = 'tools', implicit_deps = [luau_lib], dependlibs = dependlibs, libs = extralibs, configs = configs, variables = extravariables)
    #generator.bin('luauimport', ['main.c'], 'luauimport', basepath = 'tools', implicit_deps = [luau_lib], dependlibs = dependlibs, libs = extralibs, configs = configs, variables = extravariables)
    #generator.bin('luaucompile', ['main.c'], 'luaucompile', basepath = 'tools', implicit_deps = [luau_lib], dependlibs = dependlibs, libs = extralibs, configs = configs, variables = extravariables)

#No test cases if we're a submodule
if generator.is_subninja():
  sys.exit()

includepaths = generator.test_includepaths()

gllibs = []
glframeworks = []
if target.is_macos():
  glframeworks = ['OpenGL']
elif target.is_ios():
  glframeworks = ['QuartzCore', 'OpenGLES']
if target.is_windows():
  gllibs = ['opengl32', 'gdi32']
if target.is_linux():
  gllibs = ['Xxf86vm', 'Xext', 'X11', 'GL']

test_cases = [
]
if target.is_ios() or target.is_android():
  #Build one fat binary with all test cases
  test_resources = []
  test_extrasources = []
  test_cases += ['all']
  if target.is_ios():
    test_resources = [os.path.join('all', 'ios', item) for item in ['test-all.plist', 'Images.xcassets', 'test-all.xib']]
  elif target.is_android():
    test_resources = [os.path.join('all', 'android', item) for item in [
      'AndroidManifest.xml', os.path.join('layout', 'main.xml'), os.path.join('values', 'strings.xml'),
      os.path.join('drawable-ldpi', 'icon.png'), os.path.join('drawable-mdpi', 'icon.png'), os.path.join('drawable-hdpi', 'icon.png'),
      os.path.join('drawable-xhdpi', 'icon.png'), os.path.join('drawable-xxhdpi', 'icon.png'), os.path.join('drawable-xxxhdpi', 'icon.png')
    ]]
    test_extrasources = [os.path.join('all', 'android', 'java', 'com', 'maniccoder', 'foundation', 'test', item) for item in [
      'TestActivity.java'
    ]]
  if target.is_macos() or target.is_ios() or target.is_android() or target.is_tizen():
    generator.app(module = '', sources = [os.path.join(module, 'main.c') for module in test_cases] + test_extrasources, binname = 'test-all', basepath = 'test', implicit_deps = [luau_lib], libs = ['test'] + dependlibs + extralibs + gllibs, frameworks = glframeworks, resources = test_resources, includepaths = includepaths, variables = extravariables)
  else:
    generator.bin(module = '', sources = [os.path.join(module, 'main.c') for module in test_cases] + test_extrasources, binname = 'test-all', basepath = 'test', implicit_deps = [luau_lib], libs = ['test'] + dependlibs + extralibs + gllibs, frameworks = glframeworks, resources = test_resources, includepaths = includepaths, variables = extravariables)
else:
  #Build one binary per test case
  generator.bin(module = 'all', sources = ['main.c'], binname = 'test-all', basepath = 'test', implicit_deps = [luau_lib], libs = ['test'] + dependlibs + extralibs, includepaths = includepaths)
  for test in test_cases:
    generator.bin(module = test, sources = ['main.c'], binname = 'test-' + test, basepath = 'test', implicit_deps = [luau_lib], libs = ['test'] + dependlibs + extralibs + gllibs, frameworks = glframeworks, includepaths = includepaths, variables = extravariables)
