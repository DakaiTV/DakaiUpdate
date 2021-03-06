//
// url.hpp
// ~~~~~~~
//
// Copyright (c) 2011 Christopher M. Kohlhoff (chris at kohlhoff dot com)
// modify by Jack (jack.wgm@gmail.com)
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// path LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
// * $Id: url.hpp 49 2011-07-15 03:00:34Z jack $
//

#ifndef __URL_H__
#define __URL_H__

#include <iostream>
#include <string>

#include <boost/regex.hpp>
#include <boost/system/system_error.hpp>

#pragma once

/// The class @c url enables parsing and accessing the components of URLs.
/**
 * @par Example
 * To extract the components of a URL:
 * @code
 * http::url url("http://user:pass@host:1234/dir/page?param=0#anchor");
 * std::cout << "Protocol: " << url.protocol() << std::endl;
 * std::cout << "User Info: " << url.user_info() << std::endl;
 * std::cout << "Host: " << url.host() << std::endl;
 * std::cout << "Port: " << url.port() << std::endl;
 * std::cout << "Path: " << url.path() << std::endl;
 * std::cout << "Query: " << url.query() << std::endl;
 * std::cout << "Fragment: " << url.fragment() << std::endl;
 * @endcode
 * The above code will print:
 * @code
 * Protocol: http
 * User Info: user:pass
 * Host: host
 * Port: 1234
 * Path: /dir/page
 * Query: param=0
 * Fragment: anchor
 * @endcode
 *
 * @par Requirements
 * @e Header: @c <url.hpp> @n
 * @e Namespace: @c http
 */
class url
{
public:
  /// Constructs an object of class @c url.
  /**
   * @par Remarks
   * Postconditions: @c protocol(), @c user_info(), @c host(), @c path(),
   * @c query(), @c fragment() all return an empty string, and @c port() returns
   * 0.
   */
  url()
    : ipv6_host_(false)
  {
  }

  /// Constructs an object of class @c url.
  /**
   * @param s URL string to be parsed into its components.
   *
   * @throws boost::system::system_error Thrown when the URL string is invalid.
   */
  url(const char* s)
    : ipv6_host_(false)
  {
    *this = from_string(s);
  }

  /// Constructs an object of class @c url.
  /**
   * @param s URL string to be parsed into its components.
   *
   * @throws boost::system::system_error Thrown when the URL string is invalid.
   */
  url(const std::string& s)
    : ipv6_host_(false)
  {
    *this = from_string(s);
  }

  /// Gets the protocol component of the URL.
  /**
   * @returns A string specifying the protocol of the URL. Examples include
   * @c http, @c https or @c file.
   */
  std::string protocol() const
  {
    return protocol_;
  }

  /// Gets the user info component of the URL.
  /**
   * @returns A string containing the user info of the URL. Typically in the
   * format <tt>user:password</tt>, but depends on the protocol.
   */
  std::string user_info() const
  {
    return user_info_;
  }

  /// Gets the host component of the URL.
  /**
   * @returns A string containing the host name of the URL.
   */
  std::string host() const
  {
    return host_;
  }

  /// Gets the port component of the URL.
  /**
   * @returns The port number of the URL.
   *
   * @par Remarks
   * If the URL string did not specify a port, and the protocol is one of @c
   * http, @c https or @c ftp, an appropriate default port number is returned.
   */
  unsigned short port() const;

  /// Gets the path component of the URL.
  /**
   * @returns A string containing the path of the URL.
   *
   * @par Remarks
   * The path string is unescaped. To obtain the path in escaped form, use
   * @c to_string(url::path_component).
   */
  std::string path() const;

  /// Gets the filename component of the URL.
  /**
   * @returns A string containing the filename of the URL.
   *
   * @par Remarks
   * The filename string is unescaped. To obtain the filename in escaped form, use
   * @c to_string(url::filename_component).
   */
  std::string filename() const;

  /// Gets the query component of the URL.
  /**
   * @returns A string containing the query string of the URL.
   *
   * @par Remarks
   * The query string is not unescaped, but is returned in whatever form it
   * takes in the original URL string.
   */
  std::string query() const
  {
    return query_;
  }

  /// Gets the fragment component of the URL.
  /**
   * @returns A string containing the fragment of the URL.
   */
  std::string fragment() const
  {
    return fragment_;
  }

  /// Components of the URL, used with @c from_string.
  enum components_type
  {
    protocol_component = 1,
    user_info_component = 2,
    host_component = 4,
    port_component = 8,
    path_component = 16,
    query_component = 32,
    fragment_component = 64,
    all_components = protocol_component | user_info_component | host_component
      | port_component | path_component | query_component | fragment_component
  };

  /// Converts an object of class @c url to a string representation.
  /**
   * @param components A bitmask specifying which components of the URL should
   * be included in the string. See the @c url::components_type enumeration for
   * possible values.
   *
   * @returns A string representation of the URL.
   *
   * @par Examples
   * To convert the entire URL to a string:
   * @code
   * std::string s = url.to_string();
   * @endcode
   * To convert only the host and port number into a string:
   * @code
   * std::string s = url.to_string(
   *     url::host_component
   *     | url::port_component);
   * @endcode
   */
  std::string to_string(int components = all_components) const;

  /// Converts a string representation of a URL into an object of class @c url.
  /**
   * @param s URL string to be parsed into its components.
   *
   * @returns A @c url object corresponding to the specified string.
   *
   * @throws boost::system::system_error Thrown when the URL string is invalid.
   */
  static url from_string(const char* s);

  /// Converts a string representation of a URL into an object of class @c url.
  /**
   * @param s URL string to be parsed into its components.
   *
   * @param ec Error code set to indicate the reason for failure, if any.
   *
   * @returns A @c url object corresponding to the specified string.
   */
  static url from_string(const char* s,
      boost::system::error_code& ec);

  /// Converts a string representation of a URL into an object of class @c url.
  /**
   * @param s URL string to be parsed into its components.
   *
   * @returns A @c url object corresponding to the specified string.
   *
   * @throws boost::system::system_error Thrown when the URL string is invalid.
   */
  static url from_string(const std::string& s);

  /// Converts a string representation of a URL into an object of class @c url.
  /**
   * @param s URL string to be parsed into its components.
   *
   * @param ec Error code set to indicate the reason for failure, if any.
   *
   * @returns A @c url object corresponding to the specified string.
   */
   static url from_string(const std::string& s,
      boost::system::error_code& ec);

  /// Compares two @c url objects for equality.
  friend bool operator==(const url& a, const url& b);

  /// Compares two @c url objects for inequality.
  friend bool operator!=(const url& a, const url& b);

  /// Compares two @c url objects for ordering.
  friend bool operator<(const url& a, const url& b);

private:
  static bool unescape_path(const std::string& in, std::string& out);

  std::string protocol_;
  std::string user_info_;
  std::string host_;
  std::string port_;
  std::string path_;
  std::string query_;
  std::string fragment_;
  std::string file_name_;
  bool ipv6_host_;
};

#endif // __URL_H__
