#
# Licensed to the Apache Software Foundation (ASF) under one or more
# contributor license agreements.  See the NOTICE file distributed with
# this work for additional information regarding copyright ownership.
# The ASF licenses this file to You under the Apache License, Version 2.0
# (the "License"); you may not use this file except in compliance with
# the License.  You may obtain a copy of the License at
#
#       http://www.apache.org/licenses/LICENSE-2.0
#
# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS,
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
# See the License for the specific language governing permissions and
# limitations under the License.
#

# Options
option(LOG4CXX_BLOCKING_ASYNC_APPENDER "Async appender behaviour" ON)
option(LOG4CXX_ABI_CHECK "Check for ABI changes" OFF)

# Build the log4cxx library
add_library(log4cxx)
if(BUILD_SHARED_LIBS)
    target_compile_definitions(log4cxx PRIVATE LOG4CXX)
else()
    target_compile_definitions(log4cxx PUBLIC LOG4CXX_STATIC)
endif()
add_dependencies(log4cxx configure_log4cxx)
target_sources(log4cxx
  PRIVATE
  action.cpp
  andfilter.cpp
  appenderattachableimpl.cpp
  appenderskeleton.cpp
  aprinitializer.cpp
  $<IF:$<BOOL:LOG4CXX_BLOCKING_ASYNC_APPENDER>,asyncappender.cpp,asyncappender_nonblocking.cpp>
  basicconfigurator.cpp
  bufferedwriter.cpp
  bytearrayinputstream.cpp
  bytearrayoutputstream.cpp
  bytebuffer.cpp
  cacheddateformat.cpp
  charsetdecoder.cpp
  charsetencoder.cpp
  class.cpp
  classnamepatternconverter.cpp
  classregistration.cpp
  colorstartpatternconverter.cpp
  colorendpatternconverter.cpp
  configurator.cpp
  consoleappender.cpp
  cyclicbuffer.cpp
  dailyrollingfileappender.cpp
  datagrampacket.cpp
  datagramsocket.cpp
  date.cpp
  dateformat.cpp
  datelayout.cpp
  datepatternconverter.cpp
  defaultconfigurator.cpp
  defaultloggerfactory.cpp
  defaultrepositoryselector.cpp
  domconfigurator.cpp
  exception.cpp
  fallbackerrorhandler.cpp
  file.cpp
  fileappender.cpp
  filedatepatternconverter.cpp
  fileinputstream.cpp
  filelocationpatternconverter.cpp
  fileoutputstream.cpp
  filerenameaction.cpp
  filewatchdog.cpp
  filter.cpp
  filterbasedtriggeringpolicy.cpp
  fixedwindowrollingpolicy.cpp
  formattinginfo.cpp
  fulllocationpatternconverter.cpp
  gzcompressaction.cpp
  hierarchy.cpp
  htmllayout.cpp
  inetaddress.cpp
  inputstream.cpp
  inputstreamreader.cpp
  integer.cpp
  integerpatternconverter.cpp
  jsonlayout.cpp
  layout.cpp
  level.cpp
  levelmatchfilter.cpp
  levelpatternconverter.cpp
  levelrangefilter.cpp
  linelocationpatternconverter.cpp
  lineseparatorpatternconverter.cpp
  literalpatternconverter.cpp
  loader.cpp
  locale.cpp
  locationinfo.cpp
  logger.cpp
  loggermatchfilter.cpp
  loggerpatternconverter.cpp
  loggingevent.cpp
  loggingeventpatternconverter.cpp
  loglog.cpp
  logmanager.cpp
  logstream.cpp
  manualtriggeringpolicy.cpp
  mapfilter.cpp
  mdc.cpp
  messagebuffer.cpp
  messagepatternconverter.cpp
  methodlocationpatternconverter.cpp
  nameabbreviator.cpp
  namepatternconverter.cpp
  ndc.cpp
  ndcpatternconverter.cpp
  nteventlogappender.cpp
  objectoutputstream.cpp
  obsoleterollingfileappender.cpp
  odbcappender.cpp
  onlyonceerrorhandler.cpp
  optionconverter.cpp
  outputdebugstringappender.cpp
  outputstream.cpp
  outputstreamwriter.cpp
  patternconverter.cpp
  patternlayout.cpp
  patternparser.cpp
  pool.cpp
  properties.cpp
  propertiespatternconverter.cpp
  propertyconfigurator.cpp
  propertyresourcebundle.cpp
  propertysetter.cpp
  reader.cpp
  relativetimedateformat.cpp
  relativetimepatternconverter.cpp
  resourcebundle.cpp
  rollingfileappender.cpp
  rollingpolicy.cpp
  rollingpolicybase.cpp
  rolloverdescription.cpp
  rootlogger.cpp
  serversocket.cpp
  shortfilelocationpatternconverter.cpp
  simpledateformat.cpp
  simplelayout.cpp
  sizebasedtriggeringpolicy.cpp
  smtpappender.cpp
  socket.cpp
  socketappender.cpp
  socketappenderskeleton.cpp
  sockethubappender.cpp
  socketoutputstream.cpp
  strftimedateformat.cpp
  stringhelper.cpp
  stringmatchfilter.cpp
  stringtokenizer.cpp
  syslogappender.cpp
  syslogwriter.cpp
  system.cpp
  systemerrwriter.cpp
  systemoutwriter.cpp
  telnetappender.cpp
  threadlocal.cpp
  threadpatternconverter.cpp
  threadspecificdata.cpp
  threadutility.cpp
  throwableinformationpatternconverter.cpp
  timebasedrollingpolicy.cpp
  timezone.cpp
  transcoder.cpp
  transform.cpp
  triggeringpolicy.cpp
  ttcclayout.cpp
  writer.cpp
  writerappender.cpp
  xmllayout.cpp
  xmlsocketappender.cpp
  zipcompressaction.cpp
)
set_target_properties(log4cxx PROPERTIES
  VERSION ${LIBLOG4CXX_LIB_VERSION}
  SOVERSION ${LIBLOG4CXX_LIB_SOVERSION}
)
boostfallback_link(log4cxx)

if(LOG4CXX_ABI_CHECK)
    message("Getting dependencies for ABI compatability check...")
    # Get the latest version of abi-dumper and abi-compliance-checker
    include(FetchContent)
    FetchContent_Declare(abi-dumper
      GIT_REPOSITORY https://github.com/lvc/abi-dumper.git
      GIT_TAG 1.2
    )
    FetchContent_GetProperties(abi-dumper)
    if(NOT abi-dumper_POPULATED)
      FetchContent_Populate(abi-dumper)
    endif()

    FetchContent_Declare(abi-compliance-checker
      GIT_REPOSITORY https://github.com/lvc/abi-compliance-checker.git
      GIT_TAG f60ce442c33f1d5cda1cec7cfddee24af1777572
    )
    FetchContent_GetProperties(abi-compliance-checker)
    if(NOT abi-compliance-checker_POPULATED)
      FetchContent_Populate(abi-compliance-checker)
    endif()

    set(abi-dumper-script ${abi-dumper_SOURCE_DIR}/abi-dumper.pl)
    set(abi-compliance-script ${abi-compliance-checker_SOURCE_DIR}/abi-compliance-checker.pl)

    add_custom_target(dump-abi ALL
	COMMAND perl ${abi-dumper-script} -o new-abi.dump -skip-cxx -vnum ${PROJECT_VERSION_MINOR} $<TARGET_FILE:log4cxx>
	DEPENDS log4cxx
	COMMENT "Dumping ABI symbols")

    add_custom_target(compare-abi ALL
	COMMAND perl ${abi-compliance-script} -l log4cxx -old ${LOG4CXX_SOURCE_DIR}/src/main/abi-symbols/abi.dump -new new-abi.dump
	DEPENDS dump-abi
	COMMENT "Comparing ABI symbols")
endif(LOG4CXX_ABI_CHECK)
