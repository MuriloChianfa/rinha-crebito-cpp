# From MySQL fixed version
FROM mariadb:11.2.2

# Expose default MySQL port
EXPOSE 3306/tcp

# Setting default settings
ENV MARIADB_AUTO_UPGRADE 0
ENV MARIADB_INITDB_SKIP_TZINFO 1
ENV MARIADB_DISABLE_UPGRADE_BACKUP 1
ENV MARIADB_ALLOW_EMPTY_ROOT_PASSWORD 1
ENV MYSQL_ALLOW_EMPTY_PASSWORD 1

# Setting default options
ENV MYSQL_USER rinha
ENV MYSQL_DATABASE rinha

# Addind all dev scripts to entrypoint
ADD priority*-all-*.sql /docker-entrypoint-initdb.d/

# Changing permission of these scripts 
RUN chown -R mysql:mysql /docker-entrypoint-initdb.d/

# Creating MySQL featured folder
RUN mkdir -p /var/lib/mysql-files && \
    chown -R mysql:mysql /var/lib/mysql-files && \
    chmod 700 /var/lib/mysql-files

# Adding custom conf to MySQL service
COPY my.cnf /etc/mysql/my.cnf

