/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 * This file is part of the ERIS tool (https://github.com/telina/eris).
 * Copyright (c) 2023 Rhea Rinaldo (Rhea@Odlanir.de).
 *
 * Authors:
 *      - 2020 Rhea Rinaldo (Rhea@Odlanir.de)
 *      - 2021-2021 Mohammed Hussam Al Turjman (Mohammed_Hussam.Al_Turjman@dfki.de)
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, version 3.
 *
 * This program is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
 * General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program. If not, see <http://www.gnu.org/licenses/>.
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
/*
* Auto generated file by blitz 2021-03-02-18:21:16. Blitz is a tool for generating cmake
* templates, written by (Mohammed Hussam Al Turjman, hussam.turjman@gmail.com).
* For more information please visit https://github.com/Hussam-Turjman/LibraryTemplate
*/

#ifndef __LIBERIS_CONFIG_H__
#define __LIBERIS_CONFIG_H__

/* Liberis version*/
#define ERIS_VERSION_MAJOR @VERSION_MAJOR@
#define ERIS_VERSION_MINOR @VERSION_MINOR@
#define ERIS_VERSION_PATCH @VERSION_PATCH@


#define IS_POSIX 1
#define IS_WIN 0
#define IS_APPLE 0
#define IS_LINUX 1



#define HAS_UNISTD 1

#if defined(DLL_ERIS)
#   if defined(_WIN32) || defined (__CYGWIN__)
#           if defined(EXPORTING_ERIS)
#               ifdef __GNUC__
#                       define ERIS_EXPORT __attribute__ ((dllexport))
#                  else
#                       define ERIS_EXPORT __declspec(dllexport) // Note: actually gcc seems to also supports this syntax.
#               endif
#               else
#                   ifdef __GNUC__
#                           define ERIS_EXPORT __attribute__ ((dllimport))
#                       else
#                           define ERIS_EXPORT __declspec(dllimport) // Note: actually gcc seems to also supports this syntax.
#                   endif
#           endif
#           define ERIS_HIDDEN
#       else /* Not windows */
#           if __GNUC__ >= 4 || __has_attribute(visibility)
#               define ERIS_EXPORT __attribute__ ((visibility ("default")))
#               define ERIS_HIDDEN  __attribute__ ((visibility ("hidden")))
#           else
#               define ERIS_EXPORT
#               define ERIS_HIDDEN
#           endif /* __GNUC__ >= 4*/
#   endif /*WIN32*/
#else /* static build*/
#   define ERIS_EXPORT
#   define ERIS_HIDDEN
#endif /* DLL */

#if __GNUC__ >= 4 || __has_attribute(deprecated)
#define ERIS_DEPRECATED __attribute__ ((deprecated))
#else
#define ERIS_DEPRECATED
#endif

#define PRISMPATH "${PRISM}"
#define ERIS_DEBUG_MODE 1

// A macro to disallow the copy constructor and operator= functions. This should
// be used in the declarations for a class.
#define ERIS_DISALLOW_COPY_AND_ASSIGN(TypeName) \
 TypeName(const TypeName&) = delete;          \
 ERIS_DISALLOW_ASSIGN(TypeName)

// Put this in the declarations for a class to be unassignable.
#define ERIS_DISALLOW_ASSIGN(TypeName) \
 TypeName& operator=(const TypeName&) = delete;

#define ERIS_DEAD_CLASS(TypeName) \
   ERIS_DISALLOW_COPY_AND_ASSIGN(TypeName); \
   TypeName()=delete;            \
   ~TypeName()=delete

#endif /* __LIBERIS_CONFIG_H__  */

