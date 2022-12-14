# This Source Code Form is subject to the terms of the Mozilla Public
# License, v. 2.0. If a copy of the MPL was not distributed with this
# file, You can obtain one at http://mozilla.org/MPL/2.0/.


## The title and aria-label attributes are used by screen readers to describe
## the Downloads Panel.

downloads-window =
    .title = Descargas
downloads-panel =
    .aria-label = Descargas

##

# The style attribute has the width of the Downloads Panel expressed using
# a CSS unit. The longest labels that should fit are usually those of
# in-progress and blocked downloads.
downloads-panel-items =
    .style = width: 35em
downloads-cmd-pause =
    .label = Pausa
    .accesskey = P
downloads-cmd-resume =
    .label = Continuar
    .accesskey = r
downloads-cmd-cancel =
    .tooltiptext = Cancelar
downloads-cmd-cancel-panel =
    .aria-label = Cancelar
# This message is only displayed on Windows and Linux devices
downloads-cmd-show-menuitem =
    .label = Abrir carpeta contenedora
    .accesskey = A
# This message is only displayed on macOS devices
downloads-cmd-show-menuitem-mac =
    .label = Mostrar en Finder
    .accesskey = F
downloads-cmd-show-menuitem-2 =
    .label =
        { PLATFORM() ->
            [macos] Mostrar en Finder
           *[other] Mostrar en carpeta
        }
    .accesskey = r
downloads-cmd-use-system-default =
    .label = Abrir en el visor del sistema
    .accesskey = V
# We can use the same accesskey as downloads-cmd-always-open-similar-files.
# Both should not be visible in the downloads context menu at the same time.
downloads-cmd-always-use-system-default =
    .label = Abrir siempre en el visor del sistema
    .accesskey = v
downloads-cmd-show-button =
    .tooltiptext =
        { PLATFORM() ->
            [macos] Mostrar en Finder
           *[other] Abrir carpeta contenedora
        }
downloads-cmd-show-panel =
    .aria-label =
        { PLATFORM() ->
            [macos] Mostrar en Finder
           *[other] Abrir carpeta contenedora
        }
downloads-cmd-show-description =
    .value =
        { PLATFORM() ->
            [macos] Mostrar en Finder
           *[other] Abrir carpeta contenedora
        }
# We can use the same accesskey as downloads-cmd-always-use-system-default.
# Both should not be visible in the downloads context menu at the same time.
downloads-cmd-always-open-similar-files =
    .label = Siempre abrir archivos similares
    .accesskey = v
downloads-cmd-show-button-2 =
    .tooltiptext =
        { PLATFORM() ->
            [macos] Mostrar en Finder
           *[other] Mostrar en carpeta
        }
downloads-cmd-show-panel-2 =
    .aria-label =
        { PLATFORM() ->
            [macos] Mostrar en Finder
           *[other] Mostrar en carpeta
        }
downloads-cmd-show-description-2 =
    .value =
        { PLATFORM() ->
            [macos] Mostrar en Finder
           *[other] Mostrar en carpeta
        }
downloads-cmd-show-downloads =
    .label = Mostrar carpeta de descargas
downloads-cmd-retry =
    .tooltiptext = Reintentar
downloads-cmd-retry-panel =
    .aria-label = Reintentar
downloads-cmd-go-to-download-page =
    .label = Ir a la p??gina de descargas
    .accesskey = g
downloads-cmd-copy-download-link =
    .label = Copiar enlace de descarga
    .accesskey = l
downloads-cmd-remove-from-history =
    .label = Eliminar del historial
    .accesskey = E
downloads-cmd-clear-list =
    .label = Limpiar panel de vista previa
    .accesskey = a
downloads-cmd-clear-downloads =
    .label = Limpiar descargas
    .accesskey = d
# This command is shown in the context menu when downloads are blocked.
downloads-cmd-unblock =
    .label = Permitir descarga
    .accesskey = m
# This is the tooltip of the action button shown when malware is blocked.
downloads-cmd-remove-file =
    .tooltiptext = Eliminar archivo
downloads-cmd-remove-file-panel =
    .aria-label = Eliminar archivo
# This is the tooltip of the action button shown when potentially unwanted
# downloads are blocked. This opens a dialog where the user can choose
# whether to unblock or remove the download. Removing is the default option.
downloads-cmd-choose-unblock =
    .tooltiptext = Eliminar archivo o permitir descarga
downloads-cmd-choose-unblock-panel =
    .aria-label = Eliminar archivo o permitir descarga
# This is the tooltip of the action button shown when uncommon downloads are
# blocked.This opens a dialog where the user can choose whether to open the
# file or remove the download. Opening is the default option.
downloads-cmd-choose-open =
    .tooltiptext = Abrir o eliminar archivo
downloads-cmd-choose-open-panel =
    .aria-label = Abrir o eliminar archivo
# Displayed when hovering a blocked download, indicates that it's possible to
# show more information for user to take the next action.
downloads-show-more-information =
    .value = Mostrar m??s informaci??n
# Displayed when hovering a complete download, indicates that it's possible to
# open the file using an app available in the system.
downloads-open-file =
    .value = Abrir archivo

## Displayed when the user clicked on a download in process. Indicates that the
## downloading file will be opened after certain amount of time using an app
## available in the system.
## Variables:
##   $hours (number) - Amount of hours left till the file opens.
##   $seconds (number) - Amount of seconds left till the file opens.
##   $minutes (number) - Amount of minutes till the file opens.

downloading-file-opens-in-hours-and-minutes = Se abrir?? en { $hours }h { $minutes }m???
downloading-file-opens-in-minutes = Se abrir?? en { $minutes }m???
downloading-file-opens-in-minutes-and-seconds = Se abrir?? en { $minutes }m { $seconds }s???
downloading-file-opens-in-seconds = Se abrir?? en { $seconds }s???
downloading-file-opens-in-some-time = Se abrir?? cuando se complete???

##

# Displayed when hovering a download which is able to be retried by users,
# indicates that it's possible to download this file again.
downloads-retry-download =
    .value = Reintentar descarga
# Displayed when hovering a download which is able to be cancelled by users,
# indicates that it's possible to cancel and stop the download.
downloads-cancel-download =
    .value = Cancelar descarga
# This string is shown at the bottom of the Downloads Panel when all the
# downloads fit in the available space, or when there are no downloads in
# the panel at all.
downloads-history =
    .label = Mostrar todas las descargas
    .accesskey = M
# This string is shown at the top of the Download Details Panel, to indicate
# that we are showing the details of a single download.
downloads-details =
    .title = Detalles de la descarga

## Displayed when a site attempts to automatically download many files.
## Variables:
##   $num (number) - Number of blocked downloads.
##   $url (string) - The url of the suspicious site, stripped of http, https and www prefix.

downloads-files-not-downloaded =
    { $num ->
        [one] Archivo no descargado.
       *[other] { $num } archivos no descargados.
    }
downloads-blocked-from-url = Descargas bloqueadas desde { $url }.
downloads-blocked-download-detailed-info = { $url } intent?? descargar autom??ticamente m??ltiples archivos. El sitio podr??a estar da??ado o intentando almacenar archivos de spam en su dispositivo.

##

downloads-clear-downloads-button =
    .label = Limpiar descargas
    .tooltiptext = Limpia descargas completas, canceladas y fallidas
# This string is shown when there are no items in the Downloads view, when it
# is displayed inside a browser tab.
downloads-list-empty =
    .value = No hay descargas.
# This string is shown when there are no items in the Downloads Panel.
downloads-panel-empty =
    .value = No hay descargas para esta sesi??n.
# This is displayed in an item at the bottom of the Downloads Panel when there
# are more downloads than can fit in the list in the panel.
#   $count (number) - number of files being downloaded that are not shown in the
#                     panel list.
downloads-more-downloading =
    { $count ->
        [one] { $count } archivo m??s descargando
       *[other] { $count } archivos m??s descargando
    }
