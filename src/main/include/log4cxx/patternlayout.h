/*
 * Licensed to the Apache Software Foundation (ASF) under one or more
 * contributor license agreements.  See the NOTICE file distributed with
 * this work for additional information regarding copyright ownership.
 * The ASF licenses this file to You under the Apache License, Version 2.0
 * (the "License"); you may not use this file except in compliance with
 * the License.  You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#ifndef _LOG4CXX_PATTERN_LAYOUT_H
#define _LOG4CXX_PATTERN_LAYOUT_H

#if defined(_MSC_VER)
	#pragma warning ( push )
	#pragma warning ( disable: 4231 4251 4275 4786 )
#endif

#include <log4cxx/layout.h>
#include <log4cxx/pattern/loggingeventpatternconverter.h>
#include <log4cxx/pattern/formattinginfo.h>
#include <log4cxx/pattern/patternparser.h>

namespace log4cxx
{
LOG4CXX_LIST_DEF(LoggingEventPatternConverterList, log4cxx::pattern::LoggingEventPatternConverterPtr);
LOG4CXX_LIST_DEF(FormattingInfoList, log4cxx::pattern::FormattingInfoPtr);

/**
 * A flexible layout configurable with pattern string.
 *
 * <p>
 *  The goal of this class is to #format a {@link spi::LoggingEvent LoggingEvent} and
 *  return the results as a string. The results depend on the <em>conversion pattern</em>.
 * </p>
 *
 * <p>
 *  The conversion pattern is closely related to the conversion pattern of the printf
 *  function in C. A conversion pattern is composed of literal text and format control
 *  expressions called <em>conversion specifiers</em>.
 * </p>
 *
 * <p>
 *  <i>You are free to insert any literal text within the conversion pattern.</i>
 * </p>
 *
 * <p>
 *  Each conversion specifier starts with a percent sign (%) and is followed by optional
 *  <em>format modifiers</em> and a <em>conversion character</em>. The conversion character
 *  specifies the type of data, e.g. logger, level, date, thread name. The format modifiers
 *  control such things as field width, padding, left and right justification. The
 *  following is a simple example.
 * </p>
 *
 * <p>
 *  Let the conversion pattern be <strong>"%-5p [%t]: %m%n"</strong> and assume that the log4cxx
 *  environment was set to use a PatternLayout. Then the statements
 *  <pre>
 *      LoggerPtr root = Logger::getRoot();
 *      root->debug("Message 1");
 *      root->warn("Message 2");</pre>
 *  would yield the output
 *  <pre>
 *      DEBUG [main]: Message 1
 *      WARN  [main]: Message 2</pre>
 * </p>
 *
 * <p>
 *  Note that there is no explicit separator between text and conversion specifiers. The
 *  pattern parser knows when it has reached the end of a conversion specifier when it
 *  reads a conversion character. In the example above the conversion specifier <strong>%-5p</strong>
 *  means the level of the logging event should be left justified to a width of five
 *  characters.
 * </p>
 *
 * <p>The recognized conversion characters are:</p>
 *
 * <table border="1" cellpadding="8">
 *  <tr>
 *      <th align="center"><strong>Conversion Character</strong></th>
 *      <th align="center"><strong>Effect</strong></th>
 *  </tr>
 *  <tr>
 *      <td align="center"><strong>c</strong></td>
 *      <td>
 *          Used to output the logger of the logging event. The logger conversion specifier
 *          can be optionally followed by <em>precision specifier</em>, that is a decimal
 *          constant in brackets.
 *          <p>
 *              If a precision specifier is given, then only the corresponding number of
 *              right most components of the logger name will be printed. By default the
 *              logger name is printed in full.
 *          </p>
 *          <p>
 *              For example, for the logger name "a.b.c" the pattern <strong>%c{2}</strong> will
 *              output "b.c".
 *          </p>
 *      </td>
 *  </tr>
 *  <tr>
 *      <td align="center">
 *          <p><strong>C</strong></p>
 *          <p><strong>class</strong></p>
 *      </td>
 *      <td>
 *          Used to output the class of the issuer of the logging event if the compiler
 *          used supports a macro to retrieve the method of the currently compiled line and
 *          if the LOG4CXX_TRACE-like macros are used to issue a logging request. In this
 *          case the macro LOG4CXX_* is expanded at compile time to generate location info
 *          of the logging event and adds the method name, besides file and line, if
 *          available. In most cases the provided method contains the classname and can
 *          therefore be retrieved form the location info as needed.
 *          <p>
 *              Currently supported compilers are those from Microsoft, GNU-C and Borland.
 *          </p>
 *      </td>
 *  </tr>
 *  <tr>
 *      <td align="center"><strong>d</strong></td>
 *      <td>
 *          Used to output the date of the logging event. The date conversion specifier may
 *          be followed by a set of braces containing a date and time pattern string
 *          compatible with java.text.SimpleDateFormat, <em>ABSOLUTE</em>, <em>DATE</em> or
 *          <em>ISO8601</em>. For example, <strong>%d{HH:mm:ss,SSS}</strong>,
 *          <strong>%d{dd&nbsp;MMM&nbsp;yyyy&nbsp;HH:mm:ss,SSS}</strong> or <strong>%d{DATE}</strong>. If no
 *          date format specifier is given then ISO8601 format is assumed.
 *      </td>
 *  </tr>
 *  <tr>
 *      <td align="center"><strong>f</strong></td>
 *      <td>
 *          Used to output the short file name where the logging request was issued.
 *      </td>
 *  </tr>
 *  <tr>
 *      <td align="center"><strong>F</strong></td>
 *      <td>
 *          Used to output the file name where the logging request was issued.
 *      </td>
 *  </tr>
 *  <tr>
 *      <td align="center"><strong>l</strong></td>
 *      <td>
 *          Used to output location information of the caller which generated the logging
 *          event.
 *      </td>
 *  </tr>
 *  <tr>
 *      <td align="center"><strong>L</strong></td>
 *      <td>
 *          Used to output the line number from where the logging request was issued.
 *      </td>
 *  </tr>
 *  <tr>
 *      <td align="center"><strong>m</strong></td>
 *      <td>
 *          Used to output the application supplied message associated with the logging
 *          event.
 *      </td>
 *  </tr>
 *  <tr>
 *      <td align="center">
 *          <strong>M</strong>
 *          <p><strong>method</strong></p>
 *      </td>
 *      <td>
 *          Used to output the method of the issuer of the logging event if the compiler
 *          used supports a macro to retrieve the method of the currently compiled line
 *          and if the LOG4CXX_TRACE-like macros are used to issue a logging request. In
 *          this case the macro LOG4CXX_* is expanded at compile time to generate location
 *          info of the logging event and adds the method name, besides file and line, if
 *          available. In most cases the provided method contains the classname which is
 *          ignored in every attempt to retrieve the method from the location info.
 *          <p>
 *              Currently supported compilers are those from Microsoft, GNU-C and Borland.
 *          </p>
 *      </td>
 *  </tr>
 *  <tr>
 *      <td align="center"><strong>n</strong></td>
 *      <td>
 *          Outputs the platform dependent line separator character or characters.
 *          <p>
 *              This conversion character offers practically the same performance as using
 *              non-portable line separator strings such as "\n", or "\r\n". Thus, it is the
 *              preferred way of specifying a line separator.
 *          </p>
 *      </td>
 *  </tr>
 *  <tr>
 *      <td align="center"><strong>p</strong></td>
 *      <td>Used to output the level of the logging event.</td>
 *  </tr>
 *  <tr>
 *      <td align="center"><strong>r</strong></td>
 *      <td>
 *          Used to output the number of milliseconds elapsed since the start of the
 *          application until the creation of the logging event.
 *      </td>
 *  </tr>
 *  <tr>
 *      <td align="center"><strong>t</strong></td>
 *      <td>Used to output the name of the thread that generated the logging event.</td>
 *  </tr>
 *  <tr>
 *      <td align="center"><strong>x</strong></td>
 *      <td>
 *          Used to output the NDC (nested diagnostic context) associated with the thread that
 *          generated the logging event.
 *      </td>
 *  </tr>
 *  <tr>
 *      <td align="center"><strong>X</strong></td>
 *      <td>
 *          Used to output the MDC (mapped diagnostic context) associated with the thread that
 *          generated the logging event. The <strong>X</strong> conversion character <em>must</em> be
 *          followed by the key for the map placed between braces, as in <strong>%X{clientNumber}</strong>
 *          where <code>clientNumber</code> is the key. The value in the MDC corresponding to
 *          the key will be output.
 *          <p>See MDC class for more details.</p>
 *      </td>
 *  </tr>
 *  <tr>
 *      <td align="center"><strong>y</strong></td>
 *      <td>
 *          Used to wrap log with color. The <strong>y</strong> is the end of a color block.<br>
 *      </td>
 *  </tr>
 *  <tr>
 *      <td align="center"><strong>Y</strong></td>
 *      <td>
 *          Used to wrap log with color. The <strong>Y</strong> is the start of a color block.
 *          Color will be taken from the log level.
 *          - `TRACE` - blue
 *          - `DEBUG` - cyan
 *          - `INFO` - green
 *          - `WARN` - yellow
 *          - `ERROR` - red
 *          - `FATAL` - magenta
 *      </td>
 *  </tr>
 *  <tr>
 *      <td align="center"><strong>%</strong></td>
 *      <td>The sequence %% outputs a single percent sign.</td>
 *  </tr>
 * </table>
 *
 * <p>
 *  By default the relevant information is output as is. However, with the aid of format
 *  modifiers it is possible to change the minimum field width, the maximum field width
 *  and justification.
 * </p>
 *
 * <p>
 *  The optional format modifier is placed between the percent sign and the conversion
 *  character.
 * </p>
 *
 * <p>
 *  The first optional format modifier is the <em>left justification flag</em> which is
 *  just the minus (-) character. Then comes the optional <em>minimum field width</em>
 *  modifier. This is a decimal constant that represents the minimum number of characters
 *  to output. If the data item requires fewer characters, it is padded on either the left
 *  or the right until the minimum width is reached. The default is to pad on the left
 *  (right justify) but you can specify right padding with the left justification flag. The
 *  padding character is space. If the data item is larger than the minimum field width,
 *  the field is expanded to accommodate the data. The value is never truncated.
 * </p>
 *
 * <p>
 *  This behavior can be changed using the <em>maximum field width</em> modifier which is
 *  designated by a period followed by a decimal constant. If the data item is longer than
 *  the maximum field, then the extra characters are removed from the <em>beginning</em> of
 *  the data item and not from the end. For example, it the maximum field width is eight
 *  and the data item is ten characters long, then the first two characters of the data
 *  item are dropped. This behavior deviates from the printf function in C where truncation
 *  is done from the end.
 * </p>
 *
 * <p>Below are various format modifier examples for the logger conversion specifier.</p>
 *
 * <table border="1" cellpadding="8">
 *  <tr>
 *      <th align="center"><strong>Format modifier</strong></th>
 *      <th align="center"><strong>left justify</strong></th>
 *      <th align="center"><strong>minimum width</strong></th>
 *      <th align="center"><strong>maximum width</strong></th>
 *      <th align="center"><strong>comment</strong></th>
 *  </tr>
 *  <tr>
 *      <td align="center">%20c</td>
 *      <td align="center">false</td>
 *      <td align="center">20</td>
 *      <td align="center">none</td>
 *      <td>Left pad with spaces if the logger name is less than 20 characters long.</td>
 *  </tr>
 *  <tr>
 *      <td align="center">%-20c</td>
 *      <td align="center">true</td>
 *      <td align="center">20</td>
 *      <td align="center">none</td>
 *      <td>Right pad with spaces if the logger name is less than 20 characters long.</td>
 *  </tr>
 *  <tr>
 *      <td align="center">%.30c</td>
 *      <td align="center">NA</td>
 *      <td align="center">none</td>
 *      <td align="center">30</td>
 *      <td>Truncate from the beginning if the logger name is longer than 30 characters.</td>
 *  </tr>
 *  <tr>
 *      <td align="center">%20.30c</td>
 *      <td align="center">false</td>
 *      <td align="center">20</td>
 *      <td align="center">30</td>
 *      <td>
 *          Left pad with spaces if the logger name is shorter than 20 characters. However, if
 *          logger name is longer than 30 characters, then truncate from the beginning.
 *      </td>
 *  </tr>
 *  <tr>
 *      <td align="center">%-20.30c</td>
 *      <td align="center">true</td>
 *      <td align="center">20</td>
 *      <td align="center">30</td>
 *      <td>
 *          Right pad with spaces if the logger name is shorter than 20 characters. However, if
 *          logger name is longer than 30 characters, then truncate from the beginning.
 *      </td>
 *  </tr>
 * </table>
 *
 * <p>Below are some examples of conversion patterns.</p>
 *
 * <p><strong>%r [%t] %-5p %c %x - %m\n</strong></p>
 * <p>This is essentially the TTCC layout.</p>
 *
 * <p><strong>%-6r [%15.15t] %-5p %30.30c %x - %m\n</strong></p>
 *
 * <p>
 *  Similar to the TTCC layout except that the relative time is right padded if less than 6
 *  digits, thread name is right padded if less than 15 characters and truncated if longer
 *  and the logger name is left padded if shorter than 30 characters and truncated if
 *  longer.
 * </p>
 *
 * <p>
 *  The above text is largely inspired from Peter A. Darnell and Philip E. Margolis' highly
 *  recommended book "C -- a Software Engineering Approach", ISBN 0-387-97389-3.
 * </p>
 */
class LOG4CXX_EXPORT PatternLayout : public Layout
{
		/**
		 * Conversion pattern.
		 */
		LogString conversionPattern;

		/**
		 * Pattern converters.
		 */
		LoggingEventPatternConverterList patternConverters;

		/**
		 * Field widths and alignment corresponding to pattern converters.
		 */
		FormattingInfoList patternFields;

	public:
		DECLARE_LOG4CXX_OBJECT(PatternLayout)
		BEGIN_LOG4CXX_CAST_MAP()
		LOG4CXX_CAST_ENTRY(PatternLayout)
		LOG4CXX_CAST_ENTRY_CHAIN(Layout)
		END_LOG4CXX_CAST_MAP()

		/**
		 * Does nothing
		 */
		PatternLayout();

		/**
		 * Constructs a PatternLayout using the supplied conversion pattern.
		 */
		PatternLayout(const LogString& pattern);

		/**
		 * Set the <strong>ConversionPattern</strong> option. This is the string which
		 * controls formatting and consists of a mix of literal content and
		 * conversion specifiers.
		 */
		void setConversionPattern(const LogString& conversionPattern);

		/**
		 * Returns the value of the <strong>ConversionPattern</strong> option.
		 */
		inline LogString getConversionPattern() const
		{
			return conversionPattern;
		}

		/**
		 * Call createPatternParser
		 */
		virtual void activateOptions(log4cxx::helpers::Pool& p);

		virtual void setOption(const LogString& option, const LogString& value);

		/**
		 * The PatternLayout does not handle the throwable contained within
		 * {@link spi::LoggingEvent LoggingEvents}. Thus, it returns
		 * <code>true</code>.
		 */
		virtual bool ignoresThrowable() const
		{
			return true;
		}

		/**
		 * Produces a formatted string as specified by the conversion pattern.
		 */
		virtual void format(    LogString& output,
			const spi::LoggingEventPtr& event,
			log4cxx::helpers::Pool& pool) const;

	protected:
		virtual log4cxx::pattern::PatternMap getFormatSpecifiers();
};

LOG4CXX_PTR_DEF(PatternLayout);
} // namespace log4cxx

#if defined(_MSC_VER)
	#pragma warning ( pop )
#endif

#endif //_LOG4CXX_PATTERN_LAYOUT_H
